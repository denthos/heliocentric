#include "unit_manager.h"
#include "player.h"
#include "stdlib.h"
#include "logging.h"
#include "lib.h"

UnitManager::UnitManager() {}

void UnitManager::doLogic() {
	this->unit_updates.clear();

	auto& active_unit = active_units.begin();
	while (active_unit != active_units.end()) {
		Unit::CommandType command = active_unit->second->do_logic();

		if (command == Unit::UNIT_IDLE) 
		{
			active_unit->second->set_laser_shooting(false);
			this->unit_updates.insert(active_unit->second->make_update());
			idle_units.insert(std::make_pair(active_unit->first, std::move(active_unit->second)));
			active_unit = active_units.erase(active_unit);
		} 
		else if (command == Unit::UNIT_DIE) {
			this->unit_updates.insert(active_unit->second->make_update());
			active_unit->second->set_command(Unit::UNIT_IDLE);
			active_unit = active_units.erase(active_unit);
		}
		else {
			this->unit_updates.insert(active_unit->second->make_update());
			active_unit++;

		}

	}
}

std::unordered_map<UID, std::unique_ptr<Unit>>& UnitManager::get_active_units() {
	return this->active_units;
}

std::shared_ptr<UnitCreationUpdate> UnitManager::add_unit(std::shared_ptr<PlayerCommand> command, Player* player) {
	glm::vec3 unit_position(command->create_location_x, command->create_location_y, command->create_location_z);
	std::unique_ptr<Unit> new_unit = std::make_unique<Unit>(unit_position, player, 100, 100, 50, 100, 20); // Creates a new unit

	auto update = std::make_shared<UnitCreationUpdate>(new_unit->getID(),
		command->create_location_x, command->create_location_y, command->create_location_z,
		player->getID(), 100, 100, 20, 100);

	idle_units.insert(std::make_pair(new_unit->getID(), std::move(new_unit)));
	return update;
}

void UnitManager::do_move(UID id, float x, float y, float z) {
	auto& itr = idle_units.find(id);
	if (itr != idle_units.end()) {
		active_units.insert(std::make_pair(id, std::move(itr->second)));
		idle_units.erase(itr);
	}

	auto& active_units_iter = active_units.find(id);
	if (active_units_iter == active_units.end()) {
		LOG_ERR("Trying to move unit with id ", id, " but it could not be found...");
		return;
	}

	active_units_iter->second->set_destination(glm::vec3(x, y, z));
	active_units_iter->second->set_command(Unit::UNIT_MOVE);
}

void UnitManager::do_attack(UID attacker_id, UID enemy_id) {
	auto& attacker_itr = idle_units.find(attacker_id);
	auto& enemy_itr = idle_units.find(enemy_id);

	if (attacker_itr == idle_units.end()) {
		LOG_DEBUG("Attacker object already active.");
		attacker_itr = active_units.find(attacker_id);
	}
	else {
		LOG_DEBUG("Inserting attacker object to active list.");
		active_units.insert(std::make_pair(attacker_id, std::move(attacker_itr->second)));
		idle_units.erase(attacker_id);
		attacker_itr = active_units.find(attacker_id);
	}
	if (enemy_itr == idle_units.end()) {
		LOG_DEBUG("target already active.");
		enemy_itr = active_units.find(enemy_id);
	}
	else {
		LOG_DEBUG("Inserting target object to active list.");
		active_units.insert(std::make_pair(enemy_id, std::move(enemy_itr->second)));
		idle_units.erase(enemy_id);
		enemy_itr = active_units.find(enemy_id);
	}

	Lib::assertNotEqual(attacker_itr, active_units.end(), "Could not find attacking unit!");

	attacker_itr->second->set_command(Unit::UNIT_ATTACK);
	attacker_itr->second->set_combat_target(&(*(enemy_itr->second).get()));
	enemy_itr->second->set_command(Unit::UNIT_HOLDER);
}


std::unordered_set<std::shared_ptr<UnitUpdate>>& UnitManager::get_updates() {
	return this->unit_updates;
}
