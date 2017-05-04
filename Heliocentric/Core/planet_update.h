#pragma once

#include "game_object_update.h"
#include "planet.h"

class PlanetUpdate : public GameObjectUpdate {
public:
	/**
	Constructor for a planet update.
	@param id UID of the planet to be updated.
	@param x x-axis of the planet's new position.
	@param y y-axis of the planet's new position.
	@param z z-axis of the planet's new position.
	*/
	PlanetUpdate(UID, float, float, float);
	void apply(GameObject* obj);
};