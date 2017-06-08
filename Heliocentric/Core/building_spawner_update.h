#pragma once
#include "identifiable.h"
#include "building_type.h"
#include "building_spawner.h"
class BuildingSpawnerUpdate {
public:
	enum UpdateType {
		ADD_TO_QUEUE,
		POP_FROM_QUEUE,
		SPAWN_COMPLETE,
		PROGRESS_UPDATE
	};

	UpdateType updateType;
	BuildingType::TypeIdentifier buildingType;
	int percent;
	UID id;

	void apply(BuildingSpawner* spawner) {
		BuildingType* type = BuildingType::getByIdentifier(buildingType);
		switch (updateType) {
		case ADD_TO_QUEUE:
			spawner->production_queue.push_back(type);
			break;
		case POP_FROM_QUEUE:
			if (spawner->production_queue.size() > 0) {
				spawner->production_queue.erase(spawner->production_queue.begin());

				spawner->currentlyProducing = true;
				spawner->currentProduction = type;
				spawner->currentProductionProgressPercent = 0;
			}
			break;
		case PROGRESS_UPDATE:
			spawner->currentlyProducing = true;
			spawner->currentProduction = type;
			spawner->currentProductionProgressPercent = percent;
			break;
		case SPAWN_COMPLETE:
			spawner->currentlyProducing = false;
			break;
		}
	}
};