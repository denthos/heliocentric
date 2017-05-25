#pragma once
#include <set>
#include <memory>
#include <unordered_set>
#include "unit.h"
#include "unit_update.h"
#include "player_command.h"
#include "unit_creation_update.h"

class UnitManager {
private:
	std::unordered_map<UID, std::unique_ptr<Unit>> active_units;
	std::unordered_map<UID, std::unique_ptr<Unit>> idle_units;
	std::unordered_set<std::shared_ptr<UnitUpdate>> unit_updates;

public:
	UnitManager();
	void doLogic();

	std::unordered_map<UID, std::unique_ptr<Unit>>& get_active_units();
	std::shared_ptr<UnitCreationUpdate> add_unit(std::shared_ptr<PlayerCommand> command, Player* player);
	void do_move(UID id, float x, float y, float z);//add three floats 
	void do_attack(UID attacker_id, UID enemy_id);
	std::unordered_set<std::shared_ptr<UnitUpdate>>& get_updates();

	bool set_active(UID id);
	void register_update(std::shared_ptr<UnitUpdate>& update);
};
