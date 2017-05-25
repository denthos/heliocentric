#pragma once

#include "collision.h"
#include "ray.h"
#include <glm\vec3.hpp>

struct BoundingBox {
	glm::vec3 min, max;
	BoundingBox();
	BoundingBox(glm::vec3 min, glm::vec3 max);
	bool contains(const BoundingBox &);
	void expand(const glm::vec3 &);
	void expand(const BoundingBox &);
	bool intersect(const Ray &, Collision & collision = Collision()) const;
	bool collidesWith(BoundingBox other);
};