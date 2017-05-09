#include "player.h"
#include "unit.h"
#include "planet.h"
#include "city.h"
#include <iostream>
#include <string>

Player::Player(std::string player_name) : Identifiable(), name(player_name) {
	initialize();
}

Player::Player(std::string player_name, UID id) : Identifiable(id), name(player_name) {
	initialize();
}

void Player::initialize() {
	owned_objects[std::type_index(typeid(Unit))] = std::unordered_map<unsigned int, GameObject*>();
	owned_objects[std::type_index(typeid(Planet))] = std::unordered_map<unsigned int, GameObject*>();
	owned_objects[std::type_index(typeid(City))] = std::unordered_map<unsigned int, GameObject*>();
	owned_objects[std::type_index(typeid(Slot))] = std::unordered_map<unsigned int, GameObject*>();

	owned_resources[Resources::ALUMINUM] = 0.0f;
	owned_resources[Resources::GOLD] = 100.0f; // just let the player have 100 gold for now
	owned_resources[Resources::NANOMATERIAL] = 0.0f;
	owned_resources[Resources::TITANIUM] = 0.0f;
	owned_resources[Resources::URANIUM] = 0.0f;
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

float Player::get_resource_amount(Resources::ResourceType resource_type) {
	/* Assuming the parameter is a valid resource type, because it really should be. */
	return owned_resources[resource_type];
}