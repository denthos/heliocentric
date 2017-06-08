#pragma once

#include <memory>
#include "builder.h"

class BuildingSpawner : public virtual Builder {
private:
	void initialize();

protected:
	

public:
	BuildingSpawner();

	std::shared_ptr<UnitSpawnerUpdate> spawnBuilding(BuildingType::TypeIdentifier);
};