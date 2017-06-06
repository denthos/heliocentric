#pragma once
#include "fmod.hpp"
#include <string>
#include <vector>

class MusicPlayer {
public:
	MusicPlayer();
	~MusicPlayer();

	class SoundLoadError : public std::exception {};

	void load_sound(std::string);
	void play();
	void stop();

private:
	FMOD::System* system;
	std:: vector<FMOD::Sound*> sounds;

	FMOD::Channel* channel = 0;
	FMOD_RESULT result; // check result for debugging
	unsigned int version;
	void* extradriverdata = nullptr;
	bool stopped;
};