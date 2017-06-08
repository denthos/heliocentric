#pragma once

#include <atomic>
#include <memory>
#include "builder.h"
#include "identifiable.h"

class UnitSpawner : public virtual Builder {
private:
	void initialize();

protected:

public:
	UnitSpawner();

	std::shared_ptr<UnitSpawnerUpdate> spawnUnit(UnitType::TypeIdentifier);
};