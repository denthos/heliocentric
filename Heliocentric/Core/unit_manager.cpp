#include "unit_manager.h"
#include "player.h"
#include "stdlib.h"

UnitManager::UnitManager() {
	Player* p1 = new Player("player 1");
	// insert some dummy units into both idle and active units
	this->active_units.insert(std::make_unique<Unit>(glm::vec3(0.0f, 0.0f, 0.0f), p1, 25, 1, 50, 100, 0.2f));
	this->idle_units.insert(std::make_unique<Unit>(glm::vec3(0.0f, 0.0f, 0.0f), p1, 5, 1, 50, 100));

	glm::vec3 destination(0.0f, 1000.0f, 0.0f);
	for (auto& active_unit : this->active_units) {
		active_unit->set_destination(destination);
		active_unit->set_command(Unit::UNIT_MOVE);
	}
}

void UnitManager::doLogic() {
	this->unit_updates.clear();
	for (auto& active_unit : this->active_units) {
		active_unit->do_logic();
		this->unit_updates.insert(active_unit->make_update());
	}
}

std::unordered_set<std::unique_ptr<Unit>>& UnitManager::get_active_units() {
	return this->active_units;
}

std::shared_ptr<UnitCreationUpdate> UnitManager::add_unit(std::shared_ptr<PlayerCommand> command, Player* player) {
	glm::vec3 unit_position(command->create_location_x, command->create_location_y, command->create_location_z);
	std::unique_ptr<Unit> new_unit = std::make_unique<Unit>(unit_position, player, 100, 100, 20, 100); // Creates a new unit
	

	//idle_units.emplace(std::make_unique<Unit>(unit_position, player, 100, 100, 20, 100));
	//active_units.insert(unit);
	auto update = std::make_shared<UnitCreationUpdate>(new_unit->getID(),
		command->create_location_x, command->create_location_y, command->create_location_z,
		player->getID(), 100, 100, 20, 100);
	idle_units.insert(std::move(new_unit));
	return update;
}

void UnitManager::do_move(UID id, float x, float y, float z) {
}


std::unordered_set<std::shared_ptr<UnitUpdate>>& UnitManager::get_updates() {
	return this->unit_updates;
}