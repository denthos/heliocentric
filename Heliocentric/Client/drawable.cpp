#include "drawable.h"

Drawable::Drawable() : toWorld(glm::mat4(1.0f)) {

}

void Drawable::draw(const Shader & shader, const Camera & camera) const {
	model->draw(shader, camera, toWorld);
}

void Drawable::update() {
	model->update();
}

BoundingBox Drawable::getBoundingBox() const {
	BoundingBox boundingBox = model->getBoundingBox();
	glm::vec4 v = toWorld * glm::vec4(boundingBox.min, 1.0f);
	boundingBox.min = (v / v.w);
	v = toWorld * glm::vec4(boundingBox.max, 1.0f);
	boundingBox.max = (v / v.w);
	return boundingBox;
}