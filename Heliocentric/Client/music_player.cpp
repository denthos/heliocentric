#include "music_player.h"
#include "fileutils.h"
#include "logging.h"

MusicPlayer::MusicPlayer() : stopped(false) {

	/* Initialize Fmod system */
	result = FMOD::System_Create(&system);
	result = system->getVersion(&version);
	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
}

MusicPlayer::~MusicPlayer() {

}

void MusicPlayer::load_sound(std::string filename) {
	LOG_DEBUG("Load audio file: ", filename);
	FMOD::Sound* sound;
	result = system->createSound(filename.c_str(), FMOD_DEFAULT, 0, &sound);
  
	if (result != FMOD_OK) {
		throw SoundLoadError();
	}
  
	result = sound->setMode(FMOD_LOOP_OFF); // avoid auto-looping
	sounds.push_back(sound);
}

void MusicPlayer::play() {
	int num_sounds = sounds.size();
	int it = -1; // index to iterate through sounds vector
	bool channel_is_playing = false;

	while (!stopped) {
		result = channel->isPlaying(&channel_is_playing);
		if (!channel_is_playing) {
			it = (it + 1) % num_sounds; // increment iterator and go back to 0 if reached the end
			result = system->playSound(sounds[it], 0, false, &channel);
			result = system->update();
		}
	}
}

void MusicPlayer::stop() {
	stopped = true;
	LOG_INFO("Music has been stopped");
}