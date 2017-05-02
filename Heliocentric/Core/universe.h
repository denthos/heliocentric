#pragma once

#include <set>
#include <memory>

#include "planet.h"

/**
A collection of things that belong to the "map" of a game.
*/
class Universe {
private:
	std::set<std::unique_ptr<Planet>> planets;
	std::vector<std::shared_ptr<PlanetUpdate>> planet_updates;

public:
	Universe();

	void doLogic();

	std::set<std::unique_ptr<Planet>>& get_planets();
	std::vector<std::shared_ptr<PlanetUpdate>>& get_updates();
};