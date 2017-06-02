#pragma once
#include "sound_system.h"
#include "camera.h"

class ThreeDSoundSystem : public SoundSystem {
public:
	ThreeDSoundSystem(const Camera& camera);
	void update(const Camera& camera);

private:

	FMOD_VECTOR listener_pos;
	FMOD_VECTOR listener_front;
	FMOD_VECTOR listener_up;

};

void glmVecToFmodVec(const glm::vec3& glm_vec, FMOD_VECTOR& fmod_vec);