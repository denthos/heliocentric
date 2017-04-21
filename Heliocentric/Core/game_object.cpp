#include "game_object.h"

Player* GameObject::get_player() {
	return this->player;
}

glm::vec3 GameObject::get_position() {
	return this->position;
}

void GameObject::update_position(glm::vec3 new_position) {
	this->position = new_position;
}