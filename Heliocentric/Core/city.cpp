#include "city.h"
#include "player_update.h"
#include "slot_update.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include "city_manager.h"

City::City(Player* owner, Attack* attack, CityManager* manager, int def, int heal, int pr, int pop, Slot* assigned_slot, std::string name) :
	AttackableGameObject(assigned_slot->get_position(), owner, attack, def, heal), Builder(getID()), research_points(CITY_INITIAL_RESEARCH_POINTS), population(pop), slot(assigned_slot), name(name), manager(manager) {

	initialize();

}


City::City(UID id, Player* owner, Attack* attack, CityManager* manager, int def, int heal, int pr, int pop, Slot* assigned_slot, std::string name) :
	AttackableGameObject(id, assigned_slot->get_position(), owner, attack, def, heal), Builder(getID()), research_points(CITY_INITIAL_RESEARCH_POINTS), population(pop), slot(assigned_slot), name(name), manager(manager) {

	initialize();

}

void City::initialize() {
	this->update = std::make_shared<CityUpdate>(this->getID(), this->get_health(), this->get_production(), this->get_research_points());
	this->target = nullptr;
}

int City::get_population() const {
	return population;
}

void City::set_population(int new_pop) {
	population = new_pop;
}

int City::get_research_points() const {
	return research_points;
}

void City::set_research_points(int new_rp) {
	research_points = new_rp;
}

Slot* City::get_slot() {
	return slot;
}

glm::vec3 City::get_position() const {
	return slot->get_position();
}

void City::send_update_to_manager(std::shared_ptr<CityUpdate> update) {
	if (this->manager != nullptr) {
		this->manager->register_update(update);
	}
}


void City::handle_out_of_range(std::shared_ptr<AttackableGameObject> opponent) {
	// Opponent is out of range, so try next aggressor.
	this->target.reset();
}

void City::handle_victory(std::shared_ptr<AttackableGameObject> opponent) {
	// Attack your next aggressor.
	this->target.reset();
	this->player->increase_player_score(opponent->getAttack().getDamage());
}

void City::handle_defeat(std::shared_ptr<AttackableGameObject> opponent) {
	LOG_DEBUG("City with UID <", this->getID(), "> died");
	this->slot->detachCity();
	this->player->add_to_destroy(this);
	send_update_to_manager(make_update());
}

void City::handle_counter(std::shared_ptr<AttackableGameObject> opponent) {
	if (this->target == nullptr || this->target->is_dead()) {
		// Attack this opponent if your current target is dead.
		this->target.reset();
		this->target = opponent;
	}

	if (this->target->getID() == opponent->getID()) {
		LOG_DEBUG("Attacking opponent with UID <", this->target->getID(), ">");
		if (!this->do_attack(this->target)) {
			this->target.reset();
		}
	}
	send_update_to_manager(make_update());
}

std::shared_ptr<CityUpdate> City::make_update() {
	this->update->health = this->health;
	return this->update;
}

void City::extractResourcesFromSlotAndCreateUpdates(std::vector<std::shared_ptr<PlayerUpdate>>& player_updates, std::vector<std::shared_ptr<SlotUpdate>>& slot_updates) {

	if (this->is_dead()) {
		return;
	}

	int extract_amount = (this->player->getTechTree().getTechById(TECH_2) && rand() % 4 == 0) ? 4 : 2;
	LOG_DEBUG("Player <", this->player->getID(), "> extract twice: ", extract_amount);

	for (auto& resource_pair : get_slot()->getResources()) {
		if (resource_pair.second <= 0) {
			continue;
		}

		/* Don't take twice if only 1 of the resource remains. */
		int extract_amount_min = std::min(extract_amount, resource_pair.second);

		/* Change the player's resource count */
		player->change_resource_amount(resource_pair.first, extract_amount_min);
		int player_new_resource_count = player->get_resource_amount(resource_pair.first);

		/* Change the slot's resource count */
		slot->changeResourceCount(resource_pair.first, resource_pair.second - extract_amount_min);

		/* Make the player update */
		player_updates.push_back(std::make_shared<PlayerUpdate>(player->getID(), resource_pair.first, player_new_resource_count));

		/* Make the slot update */
		slot_updates.push_back(std::make_shared<SlotUpdate>(get_slot()->getID(), resource_pair.first, resource_pair.second));
	}
}

std::string City::getName() const {
	return name;
}

void City::spawnCompleteHandler(Buildable* type, Builder::ProductionType buildType) {
	switch (buildType) {
		case Builder::ProductionType::IDLE:
			LOG_ERR("Spawn completed but city was not producing anything...");
			break;
		case Builder::ProductionType::BUILDING:
			manager->handleBuildingSpawningComplete((BuildingType*) type, this);
			break;
		case Builder::ProductionType::UNIT:
			/*
			This happens when we are ready to create a unit! We need to somehow tell the unit manager.
			Let's do so through the city manager
			*/
			manager->handleUnitSpawningComplete((UnitType*) type, this);
			break;
		default:
			throw Builder::InvalidBuildTypeException();
	}
}

