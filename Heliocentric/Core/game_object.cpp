#include "game_object.h"

GameObject::GameObject(glm::vec3 pos, Player* assigned_player) : Identifiable(), position(pos), player(assigned_player) {}
GameObject::GameObject(glm::vec3 pos) : GameObject(pos, NULL) {}

GameObject::GameObject(UID id, glm::vec3 pos, Player* assigned_player) : Identifiable(id), position(pos), player(assigned_player) {}
GameObject::GameObject(UID id, glm::vec3 pos) : GameObject(id, pos, NULL) {}

Player* GameObject::get_player() {
	return this->player;
}

void GameObject::set_player(Player* assigned_player) {
	player = assigned_player;
}

const glm::vec3& GameObject::get_position() {
	return this->position;
}

void GameObject::update_position(glm::vec3 new_position) {
	this->position = new_position;
}
