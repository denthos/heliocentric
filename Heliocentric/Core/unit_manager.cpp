#include "unit_manager.h"
#include "player.h"
#include "stdlib.h"
#include "logging.h"
#include "lib.h"
#include "instant_laser_attack.h"

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
			active_unit->second->set_command(Unit::UNIT_IDLE);
			active_unit = active_units.erase(active_unit);
		}
		else {
			active_unit++;

		}

	}
}

std::unordered_map<UID, std::unique_ptr<Unit>>& UnitManager::get_active_units() {
	return this->active_units;
}

std::shared_ptr<UnitCreationUpdate> UnitManager::add_unit(std::shared_ptr<PlayerCommand> command, Player* player) {
	glm::vec3 unit_position(command->create_location_x, command->create_location_y, command->create_location_z);
	std::unique_ptr<Unit> new_unit = std::make_unique<Unit>(unit_position, player, new InstantLaserAttack(), 100, 100, 20); // Creates a new unit

	auto update = std::make_shared<UnitCreationUpdate>(new_unit->getID(),
		command->create_location_x, command->create_location_y, command->create_location_z,
		player->getID(), 100, 100, 20, 100);

	new_unit->set_manager(this);
	idle_units.insert(std::make_pair(new_unit->getID(), std::move(new_unit)));
	return update;
}

void UnitManager::do_move(UID id, float x, float y, float z) {
	LOG_DEBUG("unit ", id, " put in active list.");

	if (!set_active(id)) {
		LOG_ERR("Trying to move unit with id ", id, " but it could not be found...");
		return;
	}

	auto& itr = active_units.find(id);
	itr->second->set_destination(glm::vec3(x, y, z));
	itr->second->set_command(Unit::UNIT_MOVE);
}


bool UnitManager::set_active(UID id) {
	// Try to find unit in idle list.
	auto& it = idle_units.find(id);

	if (it == idle_units.end()) {
		LOG_DEBUG("Unit with id " + std::to_string(id) + "is already active");

	}
	else {
		// Move idle unit to active list.
		//LOG_DEBUG("Inserting Unit with id " + std::to_string(id) + " to active list");
		active_units.insert(std::make_pair(id, std::move(it->second)));
		idle_units.erase(id);
	}

	return (active_units.find(id) == active_units.end()) ? false : true;
}



void UnitManager::do_attack(UID attacker_id, UID enemy_id) {
	Lib::assertTrue(set_active(attacker_id), "Could not find attacking unit!");
	Lib::assertTrue(set_active(enemy_id), "Could not find defending unit!");
	auto& attacker_itr = active_units.find(attacker_id);
	auto& enemy_itr = active_units.find(enemy_id);
	attacker_itr->second->set_combat_target(&(*(enemy_itr->second).get()));
	attacker_itr->second->set_command(Unit::UNIT_ATTACK);
	attacker_itr->second->set_laser_shooting(true);
}


std::unordered_set<std::shared_ptr<UnitUpdate>>& UnitManager::get_updates() {
	return this->unit_updates;
}

void UnitManager::register_update(std::shared_ptr<UnitUpdate>& update) {
	this->unit_updates.insert(update);
	set_active(update->id);
}