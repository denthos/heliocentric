#include "player.h"
#include "unit.h"
#include "planet.h"
#include "city.h"
#include <iostream>
#include <string>

Player::Player(std::string player_name) : name(player_name) {
	owned_objects[std::type_index(typeid(Unit))] = std::unordered_map<unsigned int, GameObject*>();
	type_names[std::type_index(typeid(Unit))] = "Unit";
	owned_objects[std::type_index(typeid(Planet))] = std::unordered_map<unsigned int, GameObject*>();
	type_names[std::type_index(typeid(Planet))] = "Planet";
	owned_objects[std::type_index(typeid(City))] = std::unordered_map<unsigned int, GameObject*>();
	type_names[std::type_index(typeid(City))] = "City";
	owned_objects[std::type_index(typeid(Slot))] = std::unordered_map<unsigned int, GameObject*>();
	type_names[std::type_index(typeid(Slot))] = "Slot";
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

std::string Player::to_string() {
	std::string result = "Player (" + get_name() + ") ";
	for (auto it : owned_objects) {
		// only print when there is object in the map
		if (it.second.size() > 0) {
			result += type_names[it.first] + ": ";
			result += std::to_string(it.second.size());
			result += " ids: {";
			
			for (auto pair : it.second) {
				result += std::to_string(pair.first) + ";";
			}
			result += "} ";
		}
	}
	return result;
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