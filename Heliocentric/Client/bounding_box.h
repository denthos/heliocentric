#pragma once

#include <glm\vec3.hpp>

class BoundingBox {
public:
	glm::vec3 min, max;
	BoundingBox();
	BoundingBox(glm::vec3, glm::vec3);
	bool contains(const BoundingBox &);
};