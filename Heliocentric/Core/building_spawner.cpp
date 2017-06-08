#include "building_spawner.h"
#include "logging.h"

BuildingSpawner::BuildingSpawner() : Builder() {
	initialize();
}

void BuildingSpawner::initialize() {
	
}

std::shared_ptr<UnitSpawnerUpdate> BuildingSpawner::spawnBuilding(BuildingType::TypeIdentifier type) {
	auto update = std::make_shared<UnitSpawnerUpdate>();
	update->percent = 0;
	update->updateType = UnitSpawnerUpdate::UpdateType::ADD_TO_QUEUE;
	update->buildingType = type;
	update->id = id;

	/* Spawning a unit involves just putting it in the queue */
	this->production_queue.push_back(BuildingType::getByIdentifier(type));

	return update;
}
