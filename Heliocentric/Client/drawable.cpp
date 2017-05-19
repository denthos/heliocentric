#include "drawable.h"

Drawable::Drawable() : toWorld(glm::mat4(1.0f)) {

}

Drawable::~Drawable() {}

void Drawable::draw(const Shader & shader, const Camera & camera) const {
	model->draw(shader, camera, toWorld);
}

void Drawable::update() {
	model->update();
}

BoundingBox Drawable::getBoundingBox() const {
	BoundingBox boundingBox = model->getBoundingBox();
	glm::vec4 v = toWorld * glm::vec4(boundingBox.min, 1.0f);
	//boundingBox.min = (v / v.w);
	boundingBox.min = glm::vec3(v);
	v = toWorld * glm::vec4(boundingBox.max, 1.0f);
	//boundingBox.max = (v / v.w);
	boundingBox.max = glm::vec3(v);
	return boundingBox;
}

bool Drawable::intersect(const Ray & ray, Collision & collision) const {
	Ray newRay;
	Collision newCollision(collision);
	glm::mat4 inverse = glm::inverse(toWorld);
	newRay.origin = glm::vec3(inverse * glm::vec4(ray.origin, 1.0f));
	newRay.direction = glm::vec3(inverse * glm::vec4(ray.direction, 0.0f));
	if (model->getBoundingBox().intersect(newRay, newCollision)) {
		glm::vec3 collision_position = glm::vec3(toWorld * glm::vec4(newCollision.position, 1.0f)); 
		float collision_distance = glm::distance(ray.origin, collision_position);
		if (collision_distance < collision.distance) {
			collision.position = collision_position;
			collision.distance = collision_distance;
			return true;
		}
	}
	return false;
}

const glm::mat4& Drawable::getToWorld() const {
	return this->toWorld;
}

bool Drawable::do_animation(const Shader &, const Camera &) const {
	return true; 
}

