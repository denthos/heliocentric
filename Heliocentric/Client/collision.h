#pragma once

#include <limits>
#include <glm\vec3.hpp>

struct Collision {
	float distance = FLT_MAX;
	glm::vec3 position;
};