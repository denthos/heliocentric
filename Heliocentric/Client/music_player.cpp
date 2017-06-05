#include "music_player.h"
#include "fileutils.h"
#include "logging.h"

MusicPlayer::MusicPlayer(SoundSystem* system) : system(system), stopped(false) {}


MusicPlayer::~MusicPlayer() {}

void MusicPlayer::load_sound(std::string filename) {
	LOG_DEBUG("Load audio file: ", filename);

	FMOD::Sound* sound;
	FMOD_RESULT create_result = system->getSystem()->createSound(filename.c_str(), FMOD_DEFAULT, 0, &sound);
	if (create_result != FMOD_OK) {
		throw MusicLoadException("Could not load music " + filename);
	}

	sound->setMode(FMOD_LOOP_OFF); // avoid auto-looping. We will swallow the err if happens here
	sounds.push_back(sound);
}

void MusicPlayer::play() {
	int num_sounds = sounds.size();
	int it = -1; // index to iterate through sounds vector
	bool channel_is_playing = false;

	while (!stopped) {
		channel->isPlaying(&channel_is_playing);
		if (!channel_is_playing) {
			it = (it + 1) % num_sounds; // increment iterator and go back to 0 if reached the end
			FMOD_RESULT play_result = system->getSystem()->playSound(sounds[it], 0, false, &channel);
			if (play_result != FMOD_OK) {
				throw MusicPlayException();
			}
		}
	}
}

void MusicPlayer::stop() {
	stopped = true;
	LOG_INFO("Music has been stopped");
}
