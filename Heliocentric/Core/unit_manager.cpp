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

		if (command == Unit::UNIT_IDLE) {
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

Unit* UnitManager::get_unit(UID uid) const {
	auto& it = idle_units.find(uid);
	if (it == idle_units.end()) {
		auto& it = active_units.find(uid);
		if (it != active_units.end()) {
			return it->second.get();
		}
	}
	else {
		return it->second.get();
	}

	return nullptr;
}

std::unordered_map<UID, std::unique_ptr<Unit>>& UnitManager::get_active_units() {
	return this->active_units;
}

std::shared_ptr<UnitCreationUpdate> UnitManager::add_unit(std::shared_ptr<PlayerCommand> command, Player* player) {
	glm::vec3 unit_position(command->create_location_x, command->create_location_y, command->create_location_z);
	UnitType* type = UnitType::getByIdentifier(command->createUnitType);

	std::unique_ptr<Unit> new_unit = type->createUnit(unit_position, player, this);

	auto update = std::make_shared<UnitCreationUpdate>(new_unit->getID(),
		command->create_location_x, command->create_location_y, command->create_location_z,
		player->getID(), 100, 100, command->createUnitType);

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
		LOG_DEBUG("Inserting Unit with id " + std::to_string(id) + " to active list");
		active_units.insert(std::make_pair(id, std::move(it->second)));
		idle_units.erase(id);
	}

	return (active_units.find(id) == active_units.end()) ? false : true;
}



void UnitManager::do_attack(UID attacker_id, AttackableGameObject* enemy) {
	Lib::assertTrue(set_active(attacker_id), "Could not find attacking unit!");
	auto& attacker_itr = active_units.find(attacker_id);
	attacker_itr->second->set_combat_target(enemy);
	attacker_itr->second->set_command(Unit::UNIT_ATTACK);
}


std::unordered_set<std::shared_ptr<UnitUpdate>>& UnitManager::get_updates() {
	return this->unit_updates;
}

void UnitManager::register_update(std::shared_ptr<UnitUpdate>& update) {
	this->unit_updates.insert(update);
	set_active(update->id);
}
