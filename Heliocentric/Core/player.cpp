#include "player.h"
#include "player_score_update.h"
#include "player_manager.h"
#include "unit.h"
#include "planet.h"
#include "city.h"
#include "lib.h"
#include "logging.h"
#include "unit_type.h"
#include <iostream>
#include <string>

Player::Player(PlayerManager* player_manager, std::string player_name, PlayerColor::Color color) : Identifiable(), manager(player_manager), name(player_name), color(color) {
	initialize();
}

Player::Player(std::string player_name, UID id, PlayerColor::Color color) : Identifiable(id), manager(nullptr), name(player_name), color(color) {
	initialize();
}

void Player::initialize() {
	player_score = 10;
	research_points = 0.1f;
	settlement_limit = INITIAL_SETTLEMENT_LIMIT;

	owned_objects[std::type_index(typeid(Unit))] = std::unordered_map<unsigned int, GameObject*>();
	owned_objects[std::type_index(typeid(Planet))] = std::unordered_map<unsigned int, GameObject*>();
	owned_objects[std::type_index(typeid(City))] = std::unordered_map<unsigned int, GameObject*>();
	owned_objects[std::type_index(typeid(Slot))] = std::unordered_map<unsigned int, GameObject*>();

	/* Let each player have 100 of each type of resources for now */
	owned_resources[Resources::ALUMINUM] = 100;
	owned_resources[Resources::GOLD] = 100;
	owned_resources[Resources::NANOMATERIAL] = 100;
	owned_resources[Resources::TITANIUM] = 100;
	owned_resources[Resources::URANIUM] = 100;

	this->score_update = std::make_shared<PlayerScoreUpdate>(getID(), this->get_player_score());	
	this->research_update = std::make_shared<PlayerResearchUpdate>(getID(), 0, this->research_points);
}

void Player::choose_research(int id) {
	tech_tree.choose_tech(id);
}

void Player::research() {
	try {
		Technology* current_tech;
		if (tech_tree.research(this->research_points, current_tech)) {
			this->research_update->research_points = this->research_points;
			this->research_update->tech_id = current_tech->getID();
			send_update_to_manager(this->research_update);

			if (current_tech->hasResearched() && current_tech->getID() == TECH_5) {
				// Increase the settlement limit
				settlement_limit += 1;
			}
		}
	}
	catch (TechTree::ResearchIdleException) {
		return;
	}
}

void Player::research(float research_points) {
	Technology* current_tech;
	if (tech_tree.research(research_points, current_tech) && current_tech->hasResearched()) {
		if (current_tech->getID() == TECH_5) {
			// Increase the settlement limit
			settlement_limit += 1;
		}
	}
}

std::string Player::get_name() const {
	return name;
}

PlayerColor::Color Player::getColor() const {
	return this->color;
}

void Player::setColor(PlayerColor::Color color) {
	this->color = color;
}


void Player::set_name(std::string new_name) {
	name = new_name;
}

int Player::get_player_score() const {
	return player_score;
}


TechTree& Player::getTechTree() {
	return tech_tree;
}

void Player::increase_player_score(int delta) {
	player_score += delta;
	this->score_update->new_score = this->get_player_score();
	this->send_update_to_manager(this->score_update);
}

void Player::decrease_player_score(int delta) {
	player_score -= delta;
	this->score_update->new_score = this->get_player_score();
	this->send_update_to_manager(this->score_update);
}

void Player::send_update_to_manager(std::shared_ptr<PlayerScoreUpdate> update) {
	if (manager != nullptr) {
		// No manager on client side :)
		this->manager->register_update(update);
	}
}

void Player::send_update_to_manager(std::shared_ptr<PlayerResearchUpdate> update) {
	if (manager != nullptr) {
		// No manager on client side :)
		this->manager->register_update(update);
	}
}

float Player::get_research_points() {
	return research_points;
}

bool Player::can_settle() {
	return getOwnedObjects<City>().size() < settlement_limit;
}


bool Player::can_create_unit(UnitType* type) {
	return (type->hasBuildRequirements(getResources()) && type->hasTechRequirements(getTechTree()));
}

void Player::acquire_object(GameObject* object) {
	owned_objects[std::type_index(typeid(*object))].insert(std::pair<unsigned int, GameObject*>(object->getID(), object));
	// update object members
	object->set_player(this);
}

void Player::add_to_destroy(GameObject* object) {
	owned_objects[std::type_index(typeid(*object))].erase(object->getID());
}

std::unordered_map<unsigned int, GameObject*> Player::get_units() {
	return getOwnedObjects<Unit>();
}

GameObject* Player::get_unit(UID id) {
	return owned_objects[std::type_index(typeid(Unit))][id];
}

int Player::get_resource_amount(Resources::Type resource_type) {
	/* Assuming the parameter is a valid resource type, because it really should be. */
	return owned_resources[resource_type];
}

void Player::set_resource_amount(Resources::Type resource_type, int resource_amount) {
	owned_resources[resource_type] = resource_amount;
}

void Player::change_resource_amount(Resources::Type type, int delta) {
	owned_resources[type] += delta;
}

const ResourceCollection& Player::getResources() const {
	return this->owned_resources;
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
