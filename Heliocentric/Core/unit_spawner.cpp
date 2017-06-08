#include "unit_spawner.h"
#include "unit_spawner_update.h"

UnitSpawner::UnitSpawner() : Builder() {
	initialize();
}

void UnitSpawner::initialize() {

}

std::shared_ptr<UnitSpawnerUpdate> UnitSpawner::spawnUnit(UnitType::TypeIdentifier type) {
	auto update = std::make_shared<UnitSpawnerUpdate>();
	update->percent = 0;
	update->updateType = UnitSpawnerUpdate::UpdateType::ADD_TO_QUEUE;
	update->unitType = type;
	update->id = id;

	/* Spawning a unit involves just putting it in the queue */
	this->production_queue.push_back(UnitType::getByIdentifier(type));

	return update;
}
