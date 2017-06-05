#pragma once
#include "threed_sound_system.h"
#include <string>
#include <exception>

class Audio3DSound {
public:
	Audio3DSound(ThreeDSoundSystem* system, std::string filepath);

	class SoundLoadError : public std::exception {};

	void play(glm::vec3 pos);
	void update(glm::vec3 pos);

private:
	FMOD::Sound* sound;
	ThreeDSoundSystem* system;

	FMOD_VECTOR position_vector;
	FMOD::Channel* channel = NULL;
};