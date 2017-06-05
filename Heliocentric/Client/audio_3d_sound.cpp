#include "audio_3d_sound.h"

Audio3DSound::Audio3DSound(ThreeDSoundSystem* system, std::string filepath) : system(system) {
	FMOD_RESULT result = system->getSystem()->createSound(filepath.c_str(), FMOD_3D, 0, &sound);

	if (result != FMOD_OK) {
		throw SoundLoadError();
	}

	float min;
	float max;
	this->sound->get3DMinMaxDistance(&min, &max);

	// increase the volume a bit
	this->sound->set3DMinMaxDistance(min * 150.0f, max);

}

void Audio3DSound::play(glm::vec3 pos) {
	if (this->channel) {
		bool is_playing;
		this->channel->isPlaying(&is_playing);

		if (is_playing) {
			return;
		}

		this->channel->stop();
	}

	this->system->getSystem()->playSound(this->sound, 0, false, &channel);
	this->update(pos);
}

void Audio3DSound::update(glm::vec3 pos) {
	if (this->channel) {
		glmVecToFmodVec(pos, position_vector);
		this->channel->set3DAttributes(&position_vector, NULL);
	}
}