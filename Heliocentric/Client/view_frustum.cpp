#include "view_frustum.h"

bool ViewFrustum::containsOrIntersects(const BoundingBox & box) {
	float minx = box.min.x, miny = box.min.y, minz = box.min.z,
		maxx = box.max.x, maxy = box.max.y, maxz = box.max.z;
	glm::vec3 corners[8];
	// Bottom corners
	corners[0] = glm::vec3(minx, miny, minz);
	corners[1] = glm::vec3(minx, miny, maxz);
	corners[2] = glm::vec3(maxx, miny, maxz);
	corners[3] = glm::vec3(maxx, miny, minz);
	// Top corners
	corners[4] = glm::vec3(minx, maxy, minz);
	corners[5] = glm::vec3(minx, maxy, maxz);
	corners[6] = glm::vec3(maxx, maxy, maxz);
	corners[7] = glm::vec3(maxx, maxy, minz);
	
	int in = 0, out = 0;
	for (int i = 0; i < 6; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (planes[i].distance(corners[j]) < 0) {
				++out;
			}
			else {
				++in;
			}
		}
	}
	if (!in) return false;
	else return true;
}