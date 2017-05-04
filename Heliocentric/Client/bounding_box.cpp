#include "bounding_box.h"

BoundingBox::BoundingBox() : min(glm::vec3(0.0f)), max(glm::vec3(0.0f)) {

}

BoundingBox::BoundingBox(glm::vec3 min, glm::vec3 max) : min(min), max(max) {

}

bool BoundingBox::contains(const BoundingBox & box) {
	if (box.min.x >= min.x && box.min.y >= min.y && box.min.z >= min.z) {
		if (box.max.x <= max.x && box.max.y <= max.y && box.max.z >= max.z)
			return true;
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