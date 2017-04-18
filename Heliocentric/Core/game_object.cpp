#include "game_object.h"

Player* GameObject::get_player() {
	return this->player;
}

glm::vec3 GameObject::get_position() {
	return this->position;
}