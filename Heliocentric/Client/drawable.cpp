#include "drawable.h"

Drawable::Drawable() : toWorld(glm::mat4(1.0f)) {

}

void Drawable::draw(const Shader & shader, const Camera & camera) {
	mesh->draw(shader, camera, toWorld);
}

void Drawable::update() {
	mesh->update();
}

const BoundingBox & Drawable::getBoundingBox() {
	return BoundingBox();
}