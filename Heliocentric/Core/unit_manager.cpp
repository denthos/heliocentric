#include "unit_manager.h"
#include "player.h"

UnitManager::UnitManager() {
	Player* p1 = new Player("player 1");
	// insert some dummy units into both idle and active units
	this->active_units.insert(std::make_unique<Unit>(glm::vec3(0.0f, 0.0f, 0.0f), p1, 25, 1, 50, 100, 0.01f));
	this->idle_units.insert(std::make_unique<Unit>(glm::vec3(0.0f, 0.0f, 0.0f), p1, 5, 1, 50, 100));

	glm::vec3 destination(0.0f, 100.0f, 0.0f);
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

std::unordered_set<std::unique_ptr<Unit>>& UnitManager::get_units() {
	return this->active_units;
}

std::unordered_set<std::shared_ptr<UnitUpdate>>& UnitManager::get_updates() {
	return this->unit_updates;
}