#pragma once

#include "ray.h"
#include "view_frustum.h"
#include "lib.h"
#include <glm\vec3.hpp>
#include <glm\mat4x4.hpp>

#define CAMERA_SPEED_KEY "CameraSpeed"
#define CAMERA_SENSITIVITY_KEY "CameraSensitivity"
#define INVERT_Y_AXIS_KEY "InvertYAxis"

class Camera {
public:
	glm::vec3 position, target, up;
	float fov, nearDist, farDist, aspectRatio;
	int width, height;
	bool active;
	glm::mat4 view, perspective, infinite_perspective;

	ViewFrustum viewFrustum;

	Camera();
	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 up, float fov, float nearDist, float farDist, int width, int height, bool active = false);
	virtual void update() = 0;
	virtual void loadSettings(Lib::INIParser & config = Lib::INIParser::getInstance());
	glm::mat4 calculateViewMatrix();
	glm::mat4 calculatePerspectiveMatrix();
	glm::mat4 calculateInfinitePerspectiveMatrix();
	ViewFrustum calculateViewFrustum();
	void setActive(bool);

	Ray projectRay(std::pair<float, float>) const;
	Ray projectRay(float x, float y) const;
};