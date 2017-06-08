#pragma once
#include "identifiable.h"
#include "unit_type.h"
#include "building_type.h"

class UnitSpawnerUpdate {
public:
	enum UpdateType {
		ADD_TO_QUEUE,
		POP_FROM_QUEUE,
		SPAWN_COMPLETE,
		PROGRESS_UPDATE
	};

	UpdateType updateType;
	Buildable::BuildType buildType; // specifies which type of object is being built
	BuildingType::TypeIdentifier buildingType;
	UnitType::TypeIdentifier unitType;
	int percent;
	UID id;

	void apply(Builder* spawner) {
		Buildable* type;
		switch (buildType) {
			case Buildable::BuildType::BUILDING:
				type = (BuildingType*)BuildingType::getByIdentifier(buildingType);
				break;
			case Buildable::BuildType::UNIT:
				type = (UnitType*) UnitType::getByIdentifier(unitType);
				break;
		}
		
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