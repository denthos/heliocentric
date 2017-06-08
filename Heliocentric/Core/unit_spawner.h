#pragma once

#include <atomic>
#include <memory>
#include "builder.h"
#include "identifiable.h"

class UnitSpawnerUpdate;

class UnitSpawner : public virtual Builder {

	friend class UnitSpawnerUpdate;

private:
	void initialize();

	std::shared_ptr<UnitSpawnerUpdate> update;
	UID id;

	Builder::ProductionType produce();
	Builder::ProductionType popFromQueue();

protected:
	virtual void spawnCompleteHandler(Buildable* type, Builder::ProductionType buildType) = 0;

public:
	UnitSpawner(UID id);

	std::shared_ptr<UnitSpawnerUpdate> spawnUnit(UnitType::TypeIdentifier);
	std::shared_ptr<UnitSpawnerUpdate> getSpawnUpdate();
};