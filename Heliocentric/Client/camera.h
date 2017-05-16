#pragma once

#include "ray.h"
#include "view_frustum.h"
#include "lib.h"
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

class Camera {
public:
	glm::vec3 position, target, up;
	float fov, nearDist, farDist, aspectRatio;
	int width, height;
	glm::mat4 view, perspective, infinite_perspective;

	ViewFrustum viewFrustum;

	Camera();
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float nearDist, float farDist, int width, int height);
	virtual void update() = 0;
	virtual void loadSettings(Lib::INIParser & config = Lib::INIParser::getInstance());
	void calculateViewMatrix();
	void calculatePerspectiveMatrix();
	void calculateInfinitePerspectiveMatrix();
	void calculateViewFrustum();

	Ray projectRay(std::pair<float, float>) const;
	Ray projectRay(float x, float y) const;
};