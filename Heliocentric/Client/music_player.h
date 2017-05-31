#pragma once
#include "fmod.hpp"
#include <string>

class MusicPlayer {
public:
	MusicPlayer();
	~MusicPlayer();

	void load_sound(std::string);
	void play();

private:
	FMOD::System* system;
	FMOD::Sound* sound1;
	FMOD::Channel* channel = 0;
	FMOD_RESULT result; // check result for debugging
	unsigned int version;
	void* extradriverdata = nullptr;
};