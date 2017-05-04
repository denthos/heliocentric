#pragma once

#include <glm\geometric.hpp>
#include <glm\vec3.hpp>

struct Plane {
	glm::vec3 point, normal;
	Plane() : point(glm::vec3(0.0f)), normal(glm::vec3(0.0f)) {}
	Plane(glm::vec3 point, glm::vec3 normal) : point(point), normal(normal) {}
	float distance(glm::vec3 v) const {
		return glm::dot(normal, v - point);
	}
};