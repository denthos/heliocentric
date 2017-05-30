#include "city.h"
#include "player_update.h"
#include "slot_update.h"
#include <iostream>
#include <string>
#include "city_manager.h"

City::City(Player* owner, Attack* attack, CityManager* manager, int def, int heal, int pr, int pop, Slot* assigned_slot, std::string name) :
	AttackableGameObject(assigned_slot->get_position(), owner, attack, def, heal), production(pr), population(pop), slot(assigned_slot), name(name), manager(manager) {

	initialize();

}


City::City(UID id, Player* owner, Attack* attack, CityManager* manager, int def, int heal, int pr, int pop, Slot* assigned_slot, std::string name) :
	AttackableGameObject(id, assigned_slot->get_position(), owner, attack, def, heal), production(pr), population(pop), slot(assigned_slot), name(name), manager(manager) {

	initialize();

}

void City::initialize() {
	this->update = std::make_shared<CityUpdate>(this->getID(), this->get_health());
}

int City::get_population() {
	return population;
}

void City::set_population(int new_pop) {
	population = new_pop;
}

Slot* City::get_slot() {
	return slot;
}

glm::vec3 City::get_position() const {
	return slot->get_position();
}

void City::send_update_to_manager(std::shared_ptr<CityUpdate>& update) {
	if (this->manager != nullptr) {
		this->manager->register_update(update);
	}
}


void City::handle_out_of_range(AttackableGameObject* opponent) {
	// Opponent is out of range, so try next aggressor.
	this->target = nullptr;
}

void City::handle_victory(AttackableGameObject* opponent) {
	// Attack your next aggressor.
	this->target = nullptr;
}

void City::handle_defeat(AttackableGameObject* opponent) {
	this->slot->detachCity();
	send_update_to_manager(make_update());
}

void City::handle_counter(AttackableGameObject* opponent) {
	if (!(this->target) || this->target->get_health() <= 0) {
		// Attack this opponent if your current target is dead.
		this->target = opponent;
	}

	if (this->target == opponent) {
		if (!this->do_attack(this->target)) {
			this->target = nullptr;
		}
	}
	send_update_to_manager(make_update());
}

std::shared_ptr<CityUpdate> City::make_update() {
	this->update->health = this->health;
	return this->update;
}

void City::extractResourcesFromSlotAndCreateUpdates(std::vector<std::shared_ptr<PlayerUpdate>>& player_updates, std::vector<std::shared_ptr<SlotUpdate>>& slot_updates) {
	for (auto& resource_pair : get_slot()->getResources()) {

		/* Change the player's resource count */
		player->change_resource_amount(resource_pair.first, 1);
		int player_new_resource_count = player->get_resource_amount(resource_pair.first);

		/* Change the slot's resource count */
		slot->changeResourceCount(resource_pair.first, resource_pair.second - 1);

		/* Make the player update */
		player_updates.push_back(std::make_shared<PlayerUpdate>(player->getID(), resource_pair.first, player_new_resource_count));

		/* Make the slot update */
		slot_updates.push_back(std::make_shared<SlotUpdate>(get_slot()->getID(), resource_pair.first, resource_pair.second));
	}
}

std::string City::getName() const {
	return name;
}
