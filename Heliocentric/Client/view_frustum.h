#pragma once

#include "bounding_box.h"
#include "plane.h"

class ViewFrustum {
public:
	Plane planes[6];
	bool containsOrIntersects(const BoundingBox &);
};