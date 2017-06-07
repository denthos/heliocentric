#pragma once

#include <vector>
#include <atomic>
#include <memory>
#include "unit_type.h"
#include "identifiable.h"

class UnitSpawnerUpdate;

class UnitSpawner {

	friend class UnitSpawnerUpdate;

private:
	void initialize();
	std::vector<UnitType*> production_queue;

	UnitType* currentProduction;
	bool currentlyProducing;
	int currentProductionProgress;
	int currentProductionProgressPercent;

	std::shared_ptr<UnitSpawnerUpdate> update;

	bool produce();
	void popFromQueue();

	UID id;
	int production;

protected:
	virtual void spawnCompleteHandler(UnitType* type) = 0;

public:
	UnitSpawner(UID id, int production);

	const std::vector<UnitType*>& getUnitSpawnQueue();
	std::shared_ptr<UnitSpawnerUpdate> spawnUnit(UnitType::TypeIdentifier);

	bool progressSpawnAndCreateUpdate();
	std::shared_ptr<UnitSpawnerUpdate> getSpawnUpdate();

	bool isProducing() const;
	const UnitType* getCurrentProduction() const;
	int getPercentCompletion() const;

	void setProduction(int production);
	int getProduction() const;
};