#pragma once

#include "glm\glm.hpp"

class Orbit  {
public:
	Orbit() {}
	Orbit(float distance, float speed, glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f));
	void setDistance(float);
	void setSpeed(float);
	void setAxis(glm::vec3);
	float getDistance();
	float getSpeed();
	glm::vec3 getAxis();
	
	glm::mat4 getTranslateMat();
	glm::mat4 getOrbitMat();
	
protected:
	float distance, speed;
	glm::vec3 axis;
	
};

