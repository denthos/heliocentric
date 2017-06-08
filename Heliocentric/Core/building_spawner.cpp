#include "building_spawner.h"
#include "logging.h"

BuildingSpawner::BuildingSpawner(UID id) : Builder(), id(id) {
	initialize();
}

void BuildingSpawner::initialize() {
	this->update = std::make_shared<BuildingSpawnerUpdate>();
	this->update->id = id;
}

std::shared_ptr<BuildingSpawnerUpdate> BuildingSpawner::spawnBuilding(BuildingType::TypeIdentifier type) {
	auto update = std::make_shared<BuildingSpawnerUpdate>();
	update->percent = 0;
	update->updateType = BuildingSpawnerUpdate::UpdateType::ADD_TO_QUEUE;
	update->buildingType = type;
	update->id = id;

	/* Spawning a unit involves just putting it in the queue */
	this->production_queue.push_back(BuildingType::getByIdentifier(type));

	return update;
}

Builder::BuildType BuildingSpawner::popFromQueue() {
	/* Take the item from the production queue and begin processing */
	BuildingType* itemToProcess = (BuildingType*) this->production_queue.front();
	this->production_queue.erase(this->production_queue.begin());

	this->currentlyProducing = true;
	this->currentProduction = itemToProcess;
	this->currentProductionProgress = 0;

	/* Make the update */
	this->update->updateType = BuildingSpawnerUpdate::UpdateType::POP_FROM_QUEUE;
	this->update->buildingType = itemToProcess->getIdentifier();

	return Builder::BuildType::BUILDING;
}

Builder::BuildType BuildingSpawner::produce() {
	/* We are currently producing something. Let's increment it! */

	this->currentProductionProgress += (1 + this->production);

	int progressPercent = (int)(((float) this->currentProductionProgress / (float) this->currentProduction->getBuildTime()) * 100);

	/* Make the update */
	this->update->updateType = BuildingSpawnerUpdate::UpdateType::PROGRESS_UPDATE;
	this->update->buildingType = this->currentProduction->getIdentifier();
	this->update->percent = progressPercent;

	/* Are we finished? If so, we aren't producing anymore! */
	if (this->currentProductionProgress >= this->currentProduction->getBuildTime()) {

		this->update->updateType = BuildingSpawnerUpdate::UpdateType::SPAWN_COMPLETE;
		spawnCompleteHandler((BuildingType*)this->currentProduction);

		/* We need to reset stuff */
		this->currentlyProducing = false;
		this->currentProduction = NULL;
		this->currentProductionProgress = 0;
		this->currentProductionProgressPercent = 0;

		return Builder::BuildType::BUILDING;
	}
	else if (progressPercent > currentProductionProgressPercent) {
		currentProductionProgressPercent = progressPercent;
		LOG_DEBUG(progressPercent);
		return Builder::BuildType::BUILDING;
	}

	return Builder::BuildType::IDLE;
}

std::shared_ptr<BuildingSpawnerUpdate> BuildingSpawner::getSpawnUpdate() {
	return this->update;
}