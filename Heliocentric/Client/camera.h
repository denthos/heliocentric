#pragma once

#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

class Camera {
public:
	glm::vec3 position, target, up;
	float fov, nearPlane, farPlane;
	int width, height;
	glm::mat4 view, perspective;
	Camera();
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float nearPlane, float farPlane, int width, int height);
	void calculateViewMatrix();
	void calculatePerspectiveMatrix();
};