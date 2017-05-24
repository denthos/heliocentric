#include "game_object.h"

GameObject::GameObject(glm::vec3 pos, Player* assigned_player) : Identifiable(), position(pos), player(assigned_player) {}
GameObject::GameObject(glm::vec3 pos) : GameObject(pos, NULL) {}

GameObject::GameObject(UID id, glm::vec3 pos, Player* assigned_player) : Identifiable(id), position(pos), player(assigned_player) {}
GameObject::GameObject(UID id, glm::vec3 pos) : GameObject(id, pos, NULL) {}

Player* GameObject::get_player() const {
	return this->player;
}

void GameObject::set_player(Player* assigned_player) {
	player = assigned_player;
}

bool GameObject::has_player() {
	return (player != NULL);
}

glm::vec3 GameObject::get_position() const {
	return this->position;
}

void GameObject::update_position(glm::vec3 new_position) {
	this->position = new_position;
}
