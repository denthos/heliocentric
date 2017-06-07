#include "unit_spawner.h"
#include "unit_spawner_update.h"

UnitSpawner::UnitSpawner(UID id, int production) :
	currentProduction(NULL), currentlyProducing(false), currentProductionProgress(0), currentProductionProgressPercent(0), id(id), production(production) {
	initialize();
}

void UnitSpawner::initialize() {
	this->update = std::make_shared<UnitSpawnerUpdate>();
	this->update->id = id;
}


const std::vector<UnitType*>& UnitSpawner::getUnitSpawnQueue() {
	return this->production_queue;
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

bool UnitSpawner::progressSpawnAndCreateUpdate() {
	/* If there is something being processed, process it */
	if (!currentlyProducing && !this->production_queue.empty()) {
		/* There is not something processing. Let's start the processing! */
		this->popFromQueue();
		return true;
	}
	else if (currentlyProducing) {
		/* We are currently producing something.. let's work on that. */
		return this->produce();
	}
	else {
		/* The production queue is empty and we are not currently working on something. We can do nothing!*/
		return false;
	}
}

void UnitSpawner::popFromQueue() {
	/* Take the item from the production queue and begin processing */
	UnitType* itemToProcess = this->production_queue.front();
	this->production_queue.erase(this->production_queue.begin());

	this->currentlyProducing = true;
	this->currentProduction = itemToProcess;
	this->currentProductionProgress = 0;

	/* Make the update */
	this->update->updateType = UnitSpawnerUpdate::UpdateType::POP_FROM_QUEUE;
	this->update->unitType = itemToProcess->getIdentifier();
}

bool UnitSpawner::produce() {
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
		spawnCompleteHandler(this->currentProduction);

		/* We need to reset stuff */
		this->currentlyProducing = false;
		this->currentProduction = NULL;
		this->currentProductionProgress = 0;
		this->currentProductionProgressPercent = 0;

		return true;
	}
	else if (progressPercent > currentProductionProgressPercent) {
		currentProductionProgressPercent = progressPercent;
		LOG_DEBUG(progressPercent);
		return true;
	}

	return false;
}

std::shared_ptr<UnitSpawnerUpdate> UnitSpawner::getSpawnUpdate() {
	return this->update;
}

bool UnitSpawner::isProducing() const {
	return this->currentlyProducing;
}

const UnitType* UnitSpawner::getCurrentProduction() const {
	return this->currentProduction;
}

int UnitSpawner::getPercentCompletion() const {
	return this->currentProductionProgressPercent;
}

void UnitSpawner::setProduction(int production) {
	this->production = production;
}

int UnitSpawner::getProduction() const {
	return production;
}