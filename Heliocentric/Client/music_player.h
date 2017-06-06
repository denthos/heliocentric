#pragma once
#include "sound_system.h"
#include <string>
#include <stdexcept>
#include <vector>

#define MAX_VOLUME 1.0f
#define MIN_VOLUME 0.0f
#define VOLUME_STEP 0.1f

class MusicPlayer { 
public:
	MusicPlayer(SoundSystem* system);
	~MusicPlayer();

	class SoundLoadError : public std::exception {};

	void load_sound(std::string);
	void play();
	void stop();
	void volume_increase();
	void volume_decrease();

	class MusicLoadException : public std::runtime_error {
	public:
		MusicLoadException(std::string what) : std::runtime_error(what) {}
	};

	class MusicPlayException : public std::runtime_error {
	public:
		MusicPlayException() : std::runtime_error("Could not play music") {}
	};

private:
	SoundSystem* system;
	std:: vector<FMOD::Sound*> sounds;

	FMOD::Channel* channel = 0;
	void* extradriverdata = nullptr;
	bool stopped;
};
