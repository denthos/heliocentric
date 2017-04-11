#include "Group.h"

Group::~Group() {
	for (auto child : children)
		delete(child);
}

void Group::addChild(Node * child) {
	children.push_back(child);
}

void Group::removeChild(Node * child) {
	children.remove(child);
}

void Group::draw(GLuint shader, glm::mat4 C) {
	for (auto child : children)
		child->draw(shader, C);
}

void Group::update() {
	for (auto child : children)
		child->update();
}