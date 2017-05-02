#pragma once

#include <glm\geometric.hpp>
#include <glm\vec3.hpp>

struct Plane {
	glm::vec3 point, normal;
	float distance(glm::vec3 v) {
		return glm::dot(normal, v - point);
	}
};