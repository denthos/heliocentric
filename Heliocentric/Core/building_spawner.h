#pragma once

#include <memory>
#include "builder.h"
#include "building_spawner_update.h"
#include "identifiable.h"

class BuildingSpawner : public virtual Builder {

	friend class BuildingSpawnerUpdate;

private:
	void initialize();

	std::shared_ptr<BuildingSpawnerUpdate> update;
	UID id;

	Builder::ProductionType produce();
	Builder::ProductionType popFromQueue();

protected:
	virtual void spawnCompleteHandler(Buildable* type, Builder::ProductionType buildType) = 0;

public:
	BuildingSpawner(UID id);

	std::shared_ptr<BuildingSpawnerUpdate> spawnBuilding(BuildingType::TypeIdentifier);
	std::shared_ptr<BuildingSpawnerUpdate> getSpawnUpdate();
};