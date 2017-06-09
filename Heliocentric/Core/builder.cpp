#include "builder.h"
#include "unit_spawner_update.h"

Builder::Builder(UID id) : id(id), currentProduction(NULL), currentlyProducing(false),
	currentProductionProgress(0), currentProductionProgressPercent(0), production(INITIAL_PRODUCTION) {
	initialize();
}

void Builder::initialize() {
	this->update = std::make_shared<UnitSpawnerUpdate>();
	this->update->id = id;
}

const std::vector<Buildable*>& Builder::getProductionQueue() {
	return this->production_queue;
}

bool Builder::isProducing() const {
	return this->currentlyProducing;
}

const Buildable* Builder::getCurrentProduction() const {
	return this->currentProduction;
}

int Builder::getPercentCompletion() const {
	return this->currentProductionProgressPercent;
}

void Builder::set_production(int production) {
	this->production = production;
}

int Builder::get_production() const {
	return this->production;
}

Builder::ProductionType Builder::progressSpawnAndCreateUpdate() {
	/* If there is something being processed, process it */
	if (!currentlyProducing && !this->production_queue.empty()) {
		/* There is not something processing. Let's start the processing! */
		Buildable::BuildType buildType = this->popFromQueue();
		switch (buildType) {
			case Buildable::BuildType::BUILDING:
				return BUILDING;
			case Buildable::BuildType::UNIT:
				return UNIT;
		}
	}
	else if (currentlyProducing) {
		/* We are currently producing something.. let's work on that. */
		return this->produce();
	}
	else {
		/* The production queue is empty and we are not currently working on something. We can do nothing!*/
		return IDLE;
	}
}

std::shared_ptr<UnitSpawnerUpdate> Builder::getSpawnUpdate() {
	return this->update;
}

Buildable::BuildType Builder::popFromQueue() {
	/* Take the item from the production queue and begin processing */
	Buildable* itemToProcess = this->production_queue.front();
	Buildable::BuildType buildType = itemToProcess->getBuildType();
	this->production_queue.erase(this->production_queue.begin());

	this->currentlyProducing = true;
	this->currentProduction = itemToProcess;
	this->currentProductionProgress = 0;

	/* Make the update */
	this->update->updateType = UnitSpawnerUpdate::UpdateType::POP_FROM_QUEUE;
	this->update->buildType = buildType;
	switch (buildType) {
	case Buildable::BuildType::BUILDING:
		this->update->buildingType = ((BuildingType*) itemToProcess)->getIdentifier();
		break;
	case Buildable::BuildType::UNIT:
		this->update->unitType = ((UnitType*) itemToProcess)->getIdentifier();
		break;
	}

	return buildType;
}

Builder::ProductionType Builder::produce() {
	Buildable::BuildType buildType = this->currentProduction->getBuildType();

	/* We are currently producing something. Let's increment it! */
	this->currentProductionProgress += (1 + this->production);
	int progressPercent = (int)(((float) this->currentProductionProgress / (float) this->currentProduction->getProductionCost()) * 100);

	switch (buildType) {
	case Buildable::BuildType::BUILDING:
		/* Make the update */
		this->update->updateType = UnitSpawnerUpdate::UpdateType::PROGRESS_UPDATE;
		this->update->buildType = buildType;
		this->update->buildingType = ((BuildingType*) this->currentProduction)->getIdentifier();
		this->update->percent = progressPercent;

		/* Are we finished? If so, we aren't producing anymore! */
		if (this->currentProductionProgress >= this->currentProduction->getProductionCost()) {

			this->update->updateType = UnitSpawnerUpdate::UpdateType::SPAWN_COMPLETE;
			spawnCompleteHandler((BuildingType*)this->currentProduction, Builder::ProductionType::BUILDING);

			/* We need to reset stuff */
			this->currentlyProducing = false;
			this->currentProduction = NULL;
			this->currentProductionProgress = 0;
			this->currentProductionProgressPercent = 0;

			return Builder::ProductionType::BUILDING;
		}
		else if (progressPercent > currentProductionProgressPercent) {
			currentProductionProgressPercent = progressPercent;
			LOG_DEBUG(progressPercent);
			return Builder::ProductionType::BUILDING;
		}
		break;
	case Buildable::BuildType::UNIT:
		/* Make the update */
		this->update->updateType = UnitSpawnerUpdate::UpdateType::PROGRESS_UPDATE;
		this->update->buildType = buildType;
		this->update->unitType = ((UnitType*) this->currentProduction)->getIdentifier();
		this->update->percent = progressPercent;

		/* Are we finished? If so, we aren't producing anymore! */
		if (this->currentProductionProgress >= this->currentProduction->getProductionCost()) {

			this->update->updateType = UnitSpawnerUpdate::UpdateType::SPAWN_COMPLETE;
			spawnCompleteHandler((UnitType*)this->currentProduction, Builder::ProductionType::UNIT);

			/* We need to reset stuff */
			this->currentlyProducing = false;
			this->currentProduction = NULL;
			this->currentProductionProgress = 0;
			this->currentProductionProgressPercent = 0;

			return Builder::ProductionType::UNIT;
		}
		else if (progressPercent > currentProductionProgressPercent) {
			currentProductionProgressPercent = progressPercent;
			LOG_DEBUG(progressPercent);
			return Builder::ProductionType::UNIT;
		}
		break;
	}

	return Builder::ProductionType::IDLE;
}

std::shared_ptr<UnitSpawnerUpdate> Builder::spawnBuilding(BuildingType::TypeIdentifier type) {
	auto update = std::make_shared<UnitSpawnerUpdate>();
	update->percent = 0;
	update->updateType = UnitSpawnerUpdate::UpdateType::ADD_TO_QUEUE;
	update->buildingType = type;
	update->id = id;

	/* Spawning a unit involves just putting it in the queue */
	this->production_queue.push_back(BuildingType::getByIdentifier(type));

	return update;
}

std::shared_ptr<UnitSpawnerUpdate> Builder::spawnUnit(UnitType::TypeIdentifier type) {
	auto update = std::make_shared<UnitSpawnerUpdate>();
	update->percent = 0;
	update->updateType = UnitSpawnerUpdate::UpdateType::ADD_TO_QUEUE;
	update->unitType = type;
	update->id = id;

	/* Spawning a unit involves just putting it in the queue */
	this->production_queue.push_back(UnitType::getByIdentifier(type));

	return update;
}
