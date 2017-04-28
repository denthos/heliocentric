#pragma once

#include "bounding_box.h"
#include "drawable.h"
#include "view_frustum.h"
#include "camera.h"
#include <glm/vec3.hpp>
#include <queue>
#include <set>

class Octree {
public:
	Octree();
	Octree(glm::vec3 min, glm::vec3 max);
	Octree(BoundingBox region);
	Octree(BoundingBox region, std::set<Drawable *> objects);
	~Octree();
	void insert(Drawable * object);
	void clear();
	void draw(GLuint shader, const Camera & camera);
	void viewFrustumCull(ViewFrustum frustum);

protected:
	static float MIN_VOLUME;

	Octree * parent;
	BoundingBox region;
	std::set<Drawable *> objects;
	Octree * children[8];
	bool hasChildren = false;

	void update();
	void drawNode(GLuint shader, const Camera & camera);
};