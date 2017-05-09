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

	/* A wrapper around the planet's constructor that also two slots per planet */
	template <typename ... PlanetArgs>
	std::unique_ptr<Planet> createPlanet(PlanetArgs... args) {
		auto planet = std::make_unique<Planet>(args...);
		Slot* left_slot = new Slot(planet.get(), SphericalCoordinate(0, 90));
		Slot* right_slot = new Slot(planet.get(), SphericalCoordinate(180, 90));

		planet->get_slots().insert(std::make_pair(left_slot->getID(), left_slot));
		planet->get_slots().insert(std::make_pair(right_slot->getID(), right_slot));

		return planet;
	}

public:
	Universe();

	void doLogic();

	std::set<std::unique_ptr<Planet>>& get_planets();
	std::vector<std::shared_ptr<PlanetUpdate>>& get_updates();
};