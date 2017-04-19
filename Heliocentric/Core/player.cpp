#include "player.h"
#include "unit.h"
#include "planet.h"
#include "city.h"
#include <iostream>
#include <string>

Player::Player(std::string player_name, int player_ID) : name(player_name), ID(player_ID), owned_objects(std::unordered_map<std::type_index, std::vector<GameObject*>*>()) {
	owned_objects[std::type_index(typeid(Unit))] = new std::vector<GameObject*>();
	type_names[std::type_index(typeid(Unit))] = "Unit";
	owned_objects[std::type_index(typeid(Planet))] = new std::vector<GameObject*>();
	type_names[std::type_index(typeid(Planet))] = "Planet";
	owned_objects[std::type_index(typeid(City))] = new std::vector<GameObject*>();
	type_names[std::type_index(typeid(City))] = "City";
	owned_objects[std::type_index(typeid(Slot))] = new std::vector<GameObject*>();
	type_names[std::type_index(typeid(Slot))] = "Slot";
}

std::string Player::get_name() {
	return name;
}

void Player::set_name(std::string new_name) {
	name = new_name;
}

int Player::get_player_ID() {
	return ID;
}

void Player::acquire_object(GameObject* object) {
	owned_objects[std::type_index(typeid(*object))]->push_back(object);
}

void Player::print() {
	std::cout << "Player " << get_player_ID() << "(" << get_name() << ")" << std::endl;
	for (auto it : owned_objects) {
		std::cout << type_names[it.first] << ": " << (it.second)->size() << std::endl;
	}
}

std::ostream& Player::operator<< (std::ostream & out) {
	out << "Player " << get_player_ID() << "(" << get_name() << ")" << std::endl;
	return out;
}