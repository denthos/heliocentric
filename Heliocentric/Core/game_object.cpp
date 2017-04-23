#include "game_object.h"

GameObject::GameObject() {};
GameObject::GameObject(UID id) : Identifiable(id) {};
GameObject::GameObject(UID id, glm::vec3 pos) : Identifiable(id), position(pos), player(NULL) {};
GameObject::GameObject(UID id, glm::vec3 pos, Player* assigned_player) : Identifiable(id), position(pos), player(assigned_player) {};

Player* GameObject::get_player() {
	return this->player;
}

void GameObject::set_player(Player* assigned_player) {
	player = assigned_player;
}

glm::vec3 GameObject::get_position() {
	return this->position;
}

void GameObject::update_position(glm::vec3 new_position) {
	this->position = new_position;
}