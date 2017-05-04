#include "player.h"
#include "unit.h"
#include "planet.h"
#include "city.h"
#include <iostream>
#include <string>

Player::Player(std::string player_name) : Identifiable(), name(player_name) {
	owned_objects[std::type_index(typeid(Unit))] = std::unordered_map<unsigned int, GameObject*>();
	owned_objects[std::type_index(typeid(Planet))] = std::unordered_map<unsigned int, GameObject*>();
	owned_objects[std::type_index(typeid(City))] = std::unordered_map<unsigned int, GameObject*>();
	owned_objects[std::type_index(typeid(Slot))] = std::unordered_map<unsigned int, GameObject*>();
}

Player::Player(std::string player_name, UID id) : Identifiable(id), name(player_name) {
	owned_objects[std::type_index(typeid(Unit))] = std::unordered_map<unsigned int, GameObject*>();
	owned_objects[std::type_index(typeid(Planet))] = std::unordered_map<unsigned int, GameObject*>();
	owned_objects[std::type_index(typeid(City))] = std::unordered_map<unsigned int, GameObject*>();
	owned_objects[std::type_index(typeid(Slot))] = std::unordered_map<unsigned int, GameObject*>();
}

std::string Player::get_name() {
	return name;
}

void Player::set_name(std::string new_name) {
	name = new_name;
}

void Player::acquire_object(GameObject* object) {
	owned_objects[std::type_index(typeid(*object))].insert(std::pair<unsigned int, GameObject*>(object->getID(), object));
	// update object members
	object->set_player(this);
}

void Player::add_to_destroy(GameObject* object) {
	objects_to_destroy.push_back(object);
}

void Player::pop() {
	// loop through all objects for each type
	for (GameObject* object : objects_to_destroy) {
		owned_objects[std::type_index(typeid(*object))].erase(object->getID());
	}

	// clear out
	objects_to_destroy.clear();
}

std::unordered_map<unsigned int, GameObject*> Player::get_units() {
	return owned_objects[std::type_index(typeid(Unit))];
}

GameObject* Player::get_unit(UID id) {
	return owned_objects[std::type_index(typeid(Unit))][id];
}