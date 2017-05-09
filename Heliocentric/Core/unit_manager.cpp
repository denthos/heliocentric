#include "unit_manager.h"
#include "player.h"
#include "stdlib.h"
#include "logging.h"

UnitManager::UnitManager() {}

void UnitManager::doLogic() {
	this->unit_updates.clear();

	auto& active_unit = active_units.begin();
	while (active_unit != active_units.end()) {
		Unit::CommandType command = active_unit->second->do_logic();

		if (command == Unit::UNIT_IDLE) {
			idle_units.insert(std::make_pair(active_unit->first, std::move(active_unit->second)));
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
	std::unique_ptr<Unit> new_unit = std::make_unique<Unit>(unit_position, player, 100, 100, 20, 100); // Creates a new unit

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

	LOG_DEBUG("Destination is set to " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z));

	auto& active_units_iter = active_units.find(id);
	if (active_units_iter == active_units.end()) {
		LOG_ERR("Trying to move unit with id ", id, " but it could not be found...");
		return;
	}

	active_units_iter->second->set_destination(glm::vec3(x, y, z));
	active_units_iter->second->set_command(Unit::UNIT_MOVE);
}


std::unordered_set<std::shared_ptr<UnitUpdate>>& UnitManager::get_updates() {
	return this->unit_updates;
}
