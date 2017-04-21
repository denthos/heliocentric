#include "game_object.h"

Player* GameObject::get_player() {
	return this->player;
}

void GameObject::set_player(Player* assigned_player) {
	player = assigned_player;
}

glm::vec3 GameObject::get_position() {
	return this->position;
}