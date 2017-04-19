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
	// update object members
	object->set_player(this);
	object->set_vector_pos(owned_objects[std::type_index(typeid(*object))]->size()-1);
}

void Player::print() {
	std::cout << "Player " << get_player_ID() << "(" << get_name() << ")" << std::endl;
	for (auto it : owned_objects) {
		std::cout << type_names[it.first] << ": " << (it.second)->size() << std::endl;
		for (GameObject* object : (*owned_objects[it.first])) {
			std::cout << "  " << object->get_vector_pos() << std::endl;
		}
	}
}

void Player::swap_data(GameObject* object) {
	std::vector<GameObject*>* vec = owned_objects[std::type_index(typeid(*object))];
	vec->back()->set_vector_pos(object->get_vector_pos());
	vec->at(object->get_vector_pos()) = vec->back();
	vec->back() = object;
}

void Player::add_to_destory(GameObject* object) {
	objects_to_destroy.push_back(object);
}

void Player::pop() {
	// loop through all objects for each type
	for (GameObject* object : objects_to_destroy) {
		swap_data(object);
		owned_objects[std::type_index(typeid(*object))]->pop_back();
	}

	// clear out
	objects_to_destroy.clear();
}