#pragma once
#include "fmod.hpp"
#include <exception>

class SoundSystem {
public:
	SoundSystem();
	class FMODSystemCreationException : public std::exception {};

	void update();
	FMOD::System* getSystem();

private:
	FMOD::System* system;
};