#include "bounding_box.h"

#include <algorithm>
#include <glm\gtc\matrix_transform.hpp>

inline float F_MAX(float a, float b) {
	return std::max(a, b);
}

inline float F_MAX(float a, float b, float c) {
	return std::max(std::max(a, b), c);
}

inline float F_MIN(float a, float b) {
	return std::min(a, b);
}

inline float F_MIN(float a, float b, float c) {
	return std::min(std::min(a, b), c);
}

BoundingBox::BoundingBox() : min(glm::vec3(0.0f)), max(glm::vec3(0.0f)) {

}

BoundingBox::BoundingBox(glm::vec3 min, glm::vec3 max) : min(min), max(max) {

}

bool BoundingBox::contains(const BoundingBox & box) {
	if (box.min.x >= min.x && box.min.y >= min.y && box.min.z >= min.z) {
		if (box.max.x <= max.x && box.max.y <= max.y && box.max.z <= max.z) {
			return true;
		}
	}
	return false;
}

bool BoundingBox::contains(glm::vec3 point) {
	if (point.x >= min.x && point.y >= min.y && point.z >= min.z) {
		if (point.x <= max.x && point.y <= max.y && point.z <= max.z) {
			return true;
		}
	}
	return false;
}


void BoundingBox::expand(const glm::vec3 & vertex) {
	if (vertex.x < min.x) min.x = vertex.x;
	if (vertex.y < min.y) min.y = vertex.y;
	if (vertex.z < min.z) min.z = vertex.z;
	if (vertex.x > max.x) max.x = vertex.x;
	if (vertex.y > max.y) max.y = vertex.y;
	if (vertex.z > max.z) max.z = vertex.z;
}

void BoundingBox::expand(const BoundingBox & box) {
	expand(box.min);
	expand(box.max);
}

bool BoundingBox::intersect(const Ray & ray, Collision & collision) const {
	glm::vec3 invDirection = glm::vec3(1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z);
	float tx1 = (min.x - ray.origin.x) * invDirection.x,
		tx2 = (max.x - ray.origin.x) * invDirection.x,
		ty1 = (min.y - ray.origin.y) * invDirection.y,
		ty2 = (max.y - ray.origin.y) * invDirection.y,
		tz1 = (min.z - ray.origin.z) * invDirection.z,
		tz2 = (max.z - ray.origin.z) * invDirection.z;

	float tmin = F_MAX(F_MIN(tx1, tx2), F_MIN(ty1, ty2), F_MIN(tz1, tz2)),
		tmax = F_MIN(F_MAX(tx1, tx2), F_MAX(ty1, ty2), F_MAX(tz1, tz2));

	if (tmin <= tmax && tmax >= 0.0f) {
		float tval;
		if (tmin < 0.0f)
			tval = tmax;
		else
			tval = tmin;

		glm::vec3 hitPosition = ray.origin + (tval * ray.direction);
		float hitDistance = glm::distance(ray.origin, hitPosition);
		if (hitDistance < collision.distance) {
			collision.distance = hitDistance;
			collision.position = hitPosition;
			return true;
		}
	}
	return false;
}

bool BoundingBox::collidesWith(const BoundingBox& other)
{
	return (min.x <= other.max.x && max.x >= other.min.x) && 
		(min.y <= other.max.y && max.y >= other.min.y) && 
		(min.z <= other.max.z && max.z >= other.min.z) && (min != other.min && max != other.max); //CHANGE because won't detect other units
}
