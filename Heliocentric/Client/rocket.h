#pragma once
#include "model.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>

class Rocket { //TODO extend drawable unit
public:
	Rocket(Model &model);
	~Rocket();

	//animation sequences
	void TurnTowards(glm::vec3 point);
	void ShootLaser();

	

private:
	Model rocketModel;
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); //up direction is always positive y axis
	glm::vec3 pos; //this should be part of drawable unit class
	

};