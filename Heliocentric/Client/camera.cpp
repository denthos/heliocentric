#include "camera.h"

#include <glm\gtc\matrix_transform.hpp>

#define toRad (glm::pi<float>() / 180.0f)

Camera::Camera() {

}

Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float nearDist, float farDist, int width, int height) :
	position(position), target(target), up(up), fov(fov), nearDist(nearDist), farDist(farDist), width(width), height(height) {
	calculateViewMatrix();
	calculatePerspectiveMatrix();
	calculateInfinitePerspectiveMatrix();
}

void Camera::calculateViewMatrix() {
	view = glm::lookAt(position, target, up);
}

void Camera::calculatePerspectiveMatrix() {

	if (height > 0) {
		aspectRatio = (float)width / (float)height;
		perspective = glm::perspective((float)fov, aspectRatio, nearDist, farDist);
	}
}

void Camera::calculateInfinitePerspectiveMatrix()
{
	if (height > 0) {
		infinite_perspective = glm::infinitePerspective((float)fov, aspectRatio, nearDist);
	}
}


void Camera::calculateViewFrustum() {
	float tang = glm::tan(toRad * fov);
	float nearHeight = tang * nearDist;
	float nearWidth = nearHeight * aspectRatio;
	float farHeight = tang * farDist;
	float farWidth = farHeight * aspectRatio;
	glm::vec3 dir, nearCenter, farCenter, x, y, z, aux, normal;
	// z axis of camera
	z = glm::normalize(this->position - this->target);
	// x axis of camera
	x = glm::normalize(glm::cross(this->up, z));
	// real up vector
	y = glm::cross(z, x);
	// centers of near and far planes
	nearCenter = this->position - (z * nearDist);
	farCenter = this->position - (z * farDist);

	viewFrustum.planes[NEAR_PLANE] = Plane(nearCenter, -z);
	viewFrustum.planes[FAR_PLANE] = Plane(farCenter, z);

	aux = glm::normalize((nearCenter + (y * nearHeight)) - this->position);
	normal = glm::cross(aux, x);
	viewFrustum.planes[TOP_PLANE] = Plane(nearCenter + (y * nearHeight), normal);

	aux = glm::normalize((nearCenter - (y * nearHeight)) - this->position);
	normal = glm::cross(x, aux);
	viewFrustum.planes[BOTTOM_PLANE] = Plane(nearCenter - (y * nearHeight), normal);

	aux = glm::normalize((nearCenter - (x * nearWidth)) - this->position);
	normal = glm::cross(aux, y);
	viewFrustum.planes[LEFT_PLANE] = Plane(nearCenter - (x * nearWidth), normal);

	aux = glm::normalize((nearCenter + (x * nearWidth)) - this->position);
	normal = glm::cross(y, aux);
	viewFrustum.planes[RIGHT_PLANE] = Plane(nearCenter + (x * nearWidth), normal);
}

