#include "music_player.h"
#include "fileutils.h"
#include "logging.h"

MusicPlayer::MusicPlayer() {
	/* Initialize Fmod system */
	result = FMOD::System_Create(&system);
	result = system->getVersion(&version);
	result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
}

MusicPlayer::~MusicPlayer() {

}

void MusicPlayer::load_sound(std::string filename) {
	LOG_DEBUG("Load audio file: ", filename);
	result = system->createSound(filename.c_str(), FMOD_DEFAULT, 0, &sound1);
	result = sound1->setMode(FMOD_LOOP_OFF); // avoid auto-looping
}

void MusicPlayer::play() {
	result = system->playSound(sound1, 0, false, &channel);
	result = system->update();
}