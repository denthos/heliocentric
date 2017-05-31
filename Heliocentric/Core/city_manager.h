#pragma once
#include <set>
#include <memory>
#include <unordered_set>
#include "city.h"
#include "slot.h"
#include "city_update.h"
#include "settle_city_command.h"
#include "city_creation_update.h"
#include "player_command.h"
#include "unit_manager.h"

class CityManager {
private:
	std::unordered_map<UID, std::unique_ptr<City>> cities;
	std::unordered_set<std::shared_ptr<CityUpdate>> city_updates;
	std::unordered_set<std::shared_ptr<UnitSpawnerUpdate>> spawner_updates;
	std::unordered_set<std::shared_ptr<UnitCreationUpdate>> unit_creation_updates;

	UnitManager* unit_manager;

public:
	CityManager(UnitManager* unit_manager);

	void doLogic();
	City* get_city(UID uid) const;

	std::unordered_map<UID, std::unique_ptr<City>>& get_cities();

	std::shared_ptr<CityCreationUpdate> add_city(Player* player, Slot* slot, std::string name);
	std::shared_ptr<UnitSpawnerUpdate> spawnUnit(std::shared_ptr<PlayerCommand>);

	std::unordered_set<std::shared_ptr<CityUpdate>>& get_updates();
	void register_update(std::shared_ptr<CityUpdate>& update);

	const std::unordered_set<std::shared_ptr<UnitSpawnerUpdate>>& getSpawnerUpdates() const;
	const std::unordered_set<std::shared_ptr<UnitCreationUpdate>>& getCreationUpdates() const;
	void handleUnitSpawningComplete(UnitType* type, City* city);
};
