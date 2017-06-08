#pragma once

#include <atomic>
#include <memory>
#include "builder.h"
#include "unit_type.h"
#include "identifiable.h"

class UnitSpawnerUpdate;

class UnitSpawner : virtual public Builder {

	friend class UnitSpawnerUpdate;

private:
	void initialize();

	std::shared_ptr<UnitSpawnerUpdate> update;
	UID id;

	Builder::BuildType produce();
	Builder::BuildType popFromQueue();

protected:
	void spawnCompleteHandler(UnitType* type);

public:
	UnitSpawner(UID id);

	std::shared_ptr<UnitSpawnerUpdate> spawnUnit(UnitType::TypeIdentifier);
	std::shared_ptr<UnitSpawnerUpdate> getSpawnUpdate();
};