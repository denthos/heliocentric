#include "camera.h"

#include <glm\gtc\matrix_transform.hpp>

Camera::Camera() {

}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float nearPlane, float farPlane, int width, int height) :
	position(position), target(target), up(up), fov(fov), nearPlane(nearPlane), farPlane(farPlane), width(width), height(height) {
	calculateViewMatrix();
	calculatePerspectiveMatrix();
}

void Camera::calculateViewMatrix() {
	view = glm::lookAt(position, target, up);
}

void Camera::calculatePerspectiveMatrix() {
	if (height > 0)
		perspective = glm::perspective((float)fov, (float)width / (float)height, nearPlane, farPlane);
}