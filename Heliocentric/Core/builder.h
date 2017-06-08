#pragma once

#include "buildable.h"
#include "building_type.h"
#include "unit_type.h"
#include "unit_spawner_update.h"
#include <exception>
#include <vector>

#define INITIAL_PRODUCTION 1

class Builder {
	friend class UnitSpawnerUpdate;

public:
	enum ProductionType {IDLE, BUILDING, UNIT};

	Builder();

	const std::vector<Buildable*>& getProductionQueue();

	bool isProducing() const;
	const Buildable* getCurrentProduction() const;
	int getPercentCompletion() const;
	void set_production(int);
	int get_production() const;

	ProductionType progressSpawnAndCreateUpdate();
	std::shared_ptr<UnitSpawnerUpdate> getSpawnUpdate();

	class InvalidBuildTypeException : public std::exception {};

protected:
	std::shared_ptr<UnitSpawnerUpdate> update;
	UID id;

	std::vector<Buildable*> production_queue;

	Buildable* currentProduction;
	bool currentlyProducing;
	int currentProductionProgress;
	int currentProductionProgressPercent;
	int production;

	/* These two functions deal with updates, so let subclasses implement them. */
	ProductionType produce();
	Buildable::BuildType popFromQueue();

	virtual void spawnCompleteHandler(Buildable* type, Builder::ProductionType buildType) = 0;
};