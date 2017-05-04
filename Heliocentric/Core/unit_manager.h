#pragma once
#include <set>
#include <memory>
#include <unordered_set>
#include "unit.h"
#include "unit_update.h"

class UnitManager {
private:
	std::unordered_set<std::unique_ptr<Unit>> active_units;
	std::unordered_set<std::unique_ptr<Unit>> idle_units;
	std::unordered_set<std::shared_ptr<UnitUpdate>> unit_updates;

public:
	UnitManager();
	void doLogic();

	std::unordered_set<std::unique_ptr<Unit>>& get_units();
	std::unordered_set<std::shared_ptr<UnitUpdate>>& get_updates();
};
