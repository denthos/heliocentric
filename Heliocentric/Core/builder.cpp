#include "builder.h"

Builder::Builder() : currentProduction(NULL), buildType(IDLE), currentlyProducing(false),
	currentProductionProgress(0), currentProductionProgressPercent(0), production(INITIAL_PRODUCTION) {

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

Builder::BuildType Builder::progressSpawnAndCreateUpdate() {
	/* If there is something being processed, process it */
	if (!currentlyProducing && !this->production_queue.empty()) {
		/* There is not something processing. Let's start the processing! */
		return this->popFromQueue();
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