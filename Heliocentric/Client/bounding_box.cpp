#include "bounding_box.h"

BoundingBox::BoundingBox() : min(glm::vec3(0.0f)), max(glm::vec3(1.0f)) {

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