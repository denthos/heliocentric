#pragma once

#include "bounding_box.h"
#include "plane.h"

#define NEAR_PLANE 0
#define FAR_PLANE 1
#define LEFT_PLANE 2
#define RIGHT_PLANE 3
#define TOP_PLANE 4
#define BOTTOM_PLANE 5

struct ViewFrustum {
	Plane planes[6];
	bool containsOrIntersects(const BoundingBox & box) const;
};