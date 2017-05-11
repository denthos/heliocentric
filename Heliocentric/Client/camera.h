#pragma once

#include "ray.h"
#include "view_frustum.h"
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

class Camera {
public:
	glm::vec3 position, target, up;
	float fov, nearDist, farDist, aspectRatio;
	int width, height;
	glm::mat4 view, perspective;

	glm::mat4 infinite_perspective;

	ViewFrustum viewFrustum;

	Camera();
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float nearDist, float farDist, int width, int height);
	void calculateViewMatrix();
	void calculatePerspectiveMatrix();

	void calculateInfinitePerspectiveMatrix();
	void calculateViewFrustum();

	Ray projectRay(int x, int y) const;

};