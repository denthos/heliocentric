#include "unit_spawner.h"
#include "unit_spawner_update.h"

UnitSpawner::UnitSpawner(UID id) : Builder(), id(id) {
	initialize();
}

void UnitSpawner::initialize() {
	this->update = std::make_shared<UnitSpawnerUpdate>();
	this->update->id = id;
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

Builder::BuildType UnitSpawner::popFromQueue() {
	/* Take the item from the production queue and begin processing */
	UnitType* itemToProcess = (UnitType*) this->production_queue.front();
	this->production_queue.erase(this->production_queue.begin());

	this->currentlyProducing = true;
	this->currentProduction = itemToProcess;
	this->currentProductionProgress = 0;

	/* Make the update */
	this->update->updateType = UnitSpawnerUpdate::UpdateType::POP_FROM_QUEUE;
	this->update->unitType = itemToProcess->getIdentifier();

	return Builder::BuildType::UNIT;
}

Builder::BuildType UnitSpawner::produce() {
	/* We are currently producing something. Let's increment it! */

	this->currentProductionProgress += (1 + this->production);

	int progressPercent = (int)(((float) this->currentProductionProgress / (float) this->currentProduction->getBuildTime()) * 100);

	/* Make the update */
	this->update->updateType = UnitSpawnerUpdate::UpdateType::PROGRESS_UPDATE;
	this->update->unitType = this->currentProduction->getIdentifier();
	this->update->percent = progressPercent;

	/* Are we finished? If so, we aren't producing anymore! */
	if (this->currentProductionProgress >= this->currentProduction->getBuildTime()) {

		this->update->updateType = UnitSpawnerUpdate::UpdateType::SPAWN_COMPLETE;
		spawnCompleteHandler((UnitType*)this->currentProduction);

		/* We need to reset stuff */
		this->currentlyProducing = false;
		this->currentProduction = NULL;
		this->currentProductionProgress = 0;
		this->currentProductionProgressPercent = 0;

		return Builder::BuildType::UNIT;
	}
	else if (progressPercent > currentProductionProgressPercent) {
		currentProductionProgressPercent = progressPercent;
		LOG_DEBUG(progressPercent);
		return Builder::BuildType::UNIT;
	}

	return Builder::BuildType::IDLE;
}

std::shared_ptr<UnitSpawnerUpdate> UnitSpawner::getSpawnUpdate() {
	return this->update;
}
