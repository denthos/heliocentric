#include "threed_sound_system.h"

ThreeDSoundSystem::ThreeDSoundSystem(const Camera& camera) : SoundSystem() {
	this->listener_pos.x = camera.position.x;
}

void ThreeDSoundSystem::update(const Camera& camera) {
	glm::vec3 camera_front = glm::normalize(camera.position - camera.target);
	glmVecToFmodVec(camera.position, this->listener_pos);
	glmVecToFmodVec(camera_front, this->listener_front);
	glmVecToFmodVec(camera.up, this->listener_up);

	this->getSystem()->set3DListenerAttributes(0, &listener_pos, NULL, &listener_front, &listener_up);
	SoundSystem::update();
}

void glmVecToFmodVec(const glm::vec3& glm_vec, FMOD_VECTOR& fmod_vec) {
	fmod_vec.x = glm_vec.x;
	fmod_vec.y = glm_vec.y;
	fmod_vec.z = glm_vec.z;
}