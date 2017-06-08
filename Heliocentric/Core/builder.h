#pragma once

#include "buildable.h"
#include "building_type.h"
#include "unit_type.h"
#include <exception>
#include <vector>

#define INITIAL_PRODUCTION 1

class Builder {
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

	class InvalidBuildTypeException : public std::exception {};

protected:
	/* These two functions deal with updates, so let subclasses implement them. */
	virtual ProductionType produce() = 0;
	virtual ProductionType popFromQueue() = 0;

	std::vector<Buildable*> production_queue;

	Buildable* currentProduction;
	ProductionType buildType;
	bool currentlyProducing;
	int currentProductionProgress;
	int currentProductionProgressPercent;
	int production;
};