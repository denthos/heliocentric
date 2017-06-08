#pragma once

#include <memory>
#include "builder.h"
#include "building_type.h"
#include "building_spawner_update.h"
#include "identifiable.h"

class BuildingSpawner : virtual public Builder {

	friend class BuildingSpawnerUpdate;

private:
	void initialize();

	std::shared_ptr<BuildingSpawnerUpdate> update;
	UID id;

	Builder::BuildType produce();
	Builder::BuildType popFromQueue();

protected:
	void spawnCompleteHandler(BuildingType* type);

public:
	BuildingSpawner(UID id);

	std::shared_ptr<BuildingSpawnerUpdate> spawnBuilding(BuildingType::TypeIdentifier);
	std::shared_ptr<BuildingSpawnerUpdate> getSpawnUpdate();
};