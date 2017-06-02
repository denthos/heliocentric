#include "octree.h"

#include "bounding_box.h"
#include "drawable.h"
#include <glm/vec3.hpp>
#include <queue>
#include <vector>

// We don't use -FLT_MAX and FLT_MAX here to avoid hitting inf values in ray intersection
#define DEFAULT_BOUNDING_BOX_MIN glm::vec3(-1000000000000000.0f)
#define DEFAULT_BOUNDING_BOX_MAX glm::vec3( 1000000000000000.0f)
#define DEFAULT_BOUNDING_BOX BoundingBox(DEFAULT_BOUNDING_BOX_MIN, DEFAULT_BOUNDING_BOX_MAX)

float Octree::MIN_VOLUME = 1.0f;

Octree::Octree() : Octree(DEFAULT_BOUNDING_BOX) {

}

Octree::Octree(glm::vec3 min, glm::vec3 max) : Octree(BoundingBox(min, max)) {

}

Octree::Octree(BoundingBox region) : region(region) {

}

Octree::Octree(BoundingBox region, std::set<Drawable *> objects) : region(region), objects(objects) {

}

Octree::~Octree() {
	for (int i = 0; i < 8; ++i) {
		if (children[i])
			delete(children[i]);
	}
}

void Octree::insert(Drawable * object) {
	if (!shouldCull) {
		objects.insert(object);
	}
	else if (viewFrustum->containsOrIntersects(object->getBoundingBox())) {
		objects.insert(object);
	}
}

void Octree::clear() {
	objects.clear();
	for (int i = 0; i < 8; ++i) {
		if (children[i])
			children[i]->clear();
	}
}

void Octree::draw(const Shader & shader, const Camera & camera) {
	drawNode(shader, camera);
}

void Octree::drawNode(const Shader & shader, const Camera & camera) {
	// Draw this nodes objects
	for (auto it = objects.begin(); it != objects.end(); ++it) {
		(*it)->draw(shader, camera);
	}

	// Draw all child nodes
	for (int i = 0; i < 8; ++i) {
		if (children[i])
			children[i]->drawNode(shader, camera);
	}
}

void Octree::viewFrustumCull(ViewFrustum frustum) {
	if (!hasChildren)
		return;

	hasChildren = false;
	for (int i = 0; i < 8; ++i) {
		if (children[i]) {
			if (!viewFrustum->containsOrIntersects(children[i]->region)) {
				delete(children[i]);
				children[i] = NULL;
			}
			else {
				hasChildren = true;
				children[i]->viewFrustumCull(frustum);
			}
		}
	}
}

void Octree::enableViewFrustumCulling(const ViewFrustum * frustum) {
	shouldCull = true;
	this->viewFrustum = frustum;
}

void Octree::disableViewFrustumCulling() {
	shouldCull = false;
}

Drawable * Octree::intersect(const Ray & ray) {
	Drawable * intersection = NULL;
	this->intersect(ray, Collision(), intersection);
	return intersection;
}

bool Octree::intersect(const Ray & ray, Collision & collision, Drawable *& drawable) const {
	bool success = false;

	if (region.intersect(ray)) {
		// check children
		if (this->hasChildren) {
			for (int i = 0; i < 8; ++i) {
				if (children[i]) {
					if (children[i]->intersect(ray, collision, drawable)) {
						success = true;
					}
				}
			}
		}

		// check objects
		for (auto & object : objects) {
			if (object->intersect(ray, collision)) {
				drawable = object;
				success = true;
			}
		}
	}

	return success;
}

bool Octree::checkCollision(Drawable * item)
{
	bool success = false;

	if (this->hasChildren) {
		for (int i = 0; i < 8; ++i) {
			if (children[i]) {
				
				if (children[i]->checkCollision(item)) {
					success = true;
				}
					
				
			}
		}
	}

	// check objects
	for (auto & object : objects) {
		BoundingBox obstacleBB = object->getBoundingBox();
		if (item->getBoundingBox().collidesWith(obstacleBB)) {
			success = true;
		}
	}

	return success;
}

void Octree::update() {
	if (objects.size() <= 1)
		return;

	glm::vec3 dimensions = region.max - region.min;

	if (dimensions.x * dimensions.y * dimensions.z <= MIN_VOLUME)
		return;

	glm::vec3 center = region.min + (dimensions / 2.0f);

	//Create subdivided regions for each octant
	BoundingBox octant[8];
	octant[0] = BoundingBox(region.min, center);
	octant[1] = BoundingBox(glm::vec3(center.x, region.min.y, region.min.z), glm::vec3(region.max.x, center.y, center.z));
	octant[2] = BoundingBox(glm::vec3(center.x, region.min.y, center.z), glm::vec3(region.max.x, center.y, region.max.z));
	octant[3] = BoundingBox(glm::vec3(region.min.x, region.min.y, center.z), glm::vec3(center.x, center.y, region.max.z));
	octant[4] = BoundingBox(glm::vec3(region.min.x, center.y, region.min.z), glm::vec3(center.x, region.max.y, center.z));
	octant[5] = BoundingBox(glm::vec3(center.x, center.y, region.min.z), glm::vec3(region.max.x, region.max.y, center.z));
	octant[6] = BoundingBox(center, region.max);
	octant[7] = BoundingBox(glm::vec3(region.min.x, center.y, center.z), glm::vec3(center.x, region.max.y, region.max.z));

	std::set<Drawable *> objectLists[8];
	std::vector<Drawable *> delist;

	for (auto it = objects.begin(); it != objects.end(); ++it) {
		for (int i = 0; i < 8; ++i) {
			if (octant[i].contains((*it)->getBoundingBox())) {
				objectLists[i].insert(*it);
				delist.push_back(*it);
				break;
			}
		}
	}

	for (auto it = delist.begin(); it != delist.end(); ++it) {
		objects.erase(*it);
	}

	for (int i = 0; i < 8; ++i) {
		if (objectLists[i].size() > 0) {
			children[i] = new Octree(octant[i], objectLists[i]);
			children[i]->parent = this;
			hasChildren = true;
			children[i]->update();
		}
	}
}