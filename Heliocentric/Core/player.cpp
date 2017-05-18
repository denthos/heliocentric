#include "player.h"
#include "unit.h"
#include "planet.h"
#include "city.h"
#include "lib.h"
#include "logging.h"
#include <iostream>
#include <string>

Player::Player(std::string player_name) : Identifiable(), name(player_name) {
	initialize();
}

Player::Player(std::string player_name, UID id) : Identifiable(id), name(player_name) {
	initialize();
}

void Player::initialize() {
	player_score = 10.0f;

	owned_objects[std::type_index(typeid(Unit))] = std::unordered_map<unsigned int, GameObject*>();
	owned_objects[std::type_index(typeid(Planet))] = std::unordered_map<unsigned int, GameObject*>();
	owned_objects[std::type_index(typeid(City))] = std::unordered_map<unsigned int, GameObject*>();
	owned_objects[std::type_index(typeid(Slot))] = std::unordered_map<unsigned int, GameObject*>();

	/* Let each player have 100 of each type of resources for now */
	owned_resources[Resources::ALUMINUM] = 100.0f;
	owned_resources[Resources::GOLD] = 100.0f;
	owned_resources[Resources::NANOMATERIAL] = 100.0f;
	owned_resources[Resources::TITANIUM] = 100.0f;
	owned_resources[Resources::URANIUM] = 100.0f;
}

std::string Player::get_name() {
	return name;
}

void Player::set_name(std::string new_name) {
	name = new_name;
}

float Player::get_player_score() {
	return player_score;
}

void Player::increase_player_score(float delta) {
	player_score += delta;
}

void Player::decrease_player_score(float delta) {
	player_score -= delta;
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

float Player::get_resource_amount(Resources::Type resource_type) {
	/* Assuming the parameter is a valid resource type, because it really should be. */
	return owned_resources[resource_type];
}

void Player::receive_trade_deal(std::shared_ptr<TradeDeal> deal) {
	pending_trade_deals[deal->getID()] = deal;
}

UID Player::trade_deal_accept() {
	auto it = pending_trade_deals.begin();
	if (it == pending_trade_deals.end()) {
		LOG_ERR("No pending trade deal");
		return 0;
	}

	UID deal_id = it->first;
	trade_deal_accept(deal_id);
	return deal_id;
}

UID Player::trade_deal_decline() {
	auto it = pending_trade_deals.begin();
	if (it == pending_trade_deals.end()) {
		LOG_ERR("No pending trade deal");
		return 0;
	}

	UID deal_id = it->first;
	trade_deal_decline(deal_id);
	return deal_id;
}

void Player::trade_deal_accept(UID id) {
	if (pending_trade_deals.find(id) == pending_trade_deals.end()) {
		LOG_ERR("Invalid trade deal ID");
		return;
	}

	pending_trade_deals.erase(id); // remove deal from pending list
}

void Player::trade_deal_decline(UID id) {
	if (pending_trade_deals.find(id) == pending_trade_deals.end()) {
		LOG_ERR("Invalid trade deal ID");
		return;
	}

	pending_trade_deals.erase(id); // remove deal from pending list
}

std::shared_ptr<TradeDeal> Player::get_trade_deal() {
	auto it = pending_trade_deals.begin();
	if (it == pending_trade_deals.end()) {
		/* Just do this for now even though it's not really a bad UID */
		throw BadUIDException();
	}

	return it->second;
}

std::shared_ptr<TradeDeal> Player::get_trade_deal(UID id) {
	if (pending_trade_deals.find(id) == pending_trade_deals.end()) {
		throw BadUIDException();
	}

	return pending_trade_deals[id];
}