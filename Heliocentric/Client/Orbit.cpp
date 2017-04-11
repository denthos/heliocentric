#include "Orbit.h"

#include <glm\gtc\constants.hpp>
#include <glm\gtc\matrix_transform.hpp>

Orbit::Orbit(float distance, float speed, glm::vec3 axis) : distance(distance), speed(speed), axis(axis) {
	M = glm::translate(glm::mat4(1.0f), glm::vec3(distance, 0.0f, 0.0f));
}

void Orbit::setDistance(float distance) {
	this->distance = distance;
}

void Orbit::setSpeed(float speed) {
	this->speed = speed;
}

void Orbit::setAxis(glm::vec3 axis) {
	this->axis = axis;
}

float Orbit::getSpeed() {
	return speed;
}

glm::vec3 Orbit::getAxis() {
	return axis;
}

glm::mat4 Orbit::getTransformation() {
	return M;
}

void Orbit::draw(GLuint shader, glm::mat4 C) {
	Group::draw(shader, C * M);
}

void Orbit::update() {
	M = glm::rotate(glm::mat4(1.0f), (speed / 180.0f) * glm::pi<float>(), axis) * M;
	Group::update();
}
