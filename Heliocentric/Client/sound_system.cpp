#include "sound_system.h"

SoundSystem::SoundSystem() {
	FMOD_RESULT system_creation_result = FMOD::System_Create(&system);
	if (system_creation_result != FMOD_OK) {
		throw FMODSystemCreationException();
	}

	system_creation_result = system->init(32, FMOD_INIT_NORMAL, NULL);
	if (system_creation_result != FMOD_OK) {
		throw FMODSystemCreationException();
	}
}

void SoundSystem::update() {
	system->update();
}

FMOD::System* SoundSystem::getSystem() {
	return system;
}