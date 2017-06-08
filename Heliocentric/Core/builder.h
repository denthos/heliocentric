#pragma once

#include "buildable.h"
#include <vector>

#define INITIAL_PRODUCTION 1

class Builder {
public:
	enum BuildType {IDLE, BUILDING, UNIT};

	Builder();

	const std::vector<Buildable*>& getProductionQueue();

	bool isProducing() const;
	const Buildable* getCurrentProduction() const;
	int getPercentCompletion() const;
	void set_production(int);
	int get_production() const;

	BuildType progressSpawnAndCreateUpdate();

protected:
	/* These two functions deal with updates, so let subclasses implement them. */
	virtual BuildType produce() = 0;
	virtual BuildType popFromQueue() = 0;

	std::vector<Buildable*> production_queue;

	Buildable* currentProduction;
	BuildType buildType;
	bool currentlyProducing;
	int currentProductionProgress;
	int currentProductionProgressPercent;
	int production;
};