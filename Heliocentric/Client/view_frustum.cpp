#include "view_frustum.h"

bool ViewFrustum::containsOrIntersects(const BoundingBox & box) const {
	for (int i = 0; i < 6; ++i) {
		glm::vec3 p = box.min;
		if (planes[i].normal.x >= 0.0f)
			p.x = box.max.x;
		if (planes[i].normal.y >= 0.0f)
			p.y = box.max.y;
		if (planes[i].normal.z >= 0.0f)
			p.z = box.max.z;

		if (planes[i].distance(p) < 0.0f) {
			return false;
		}
	}

	return true;
}