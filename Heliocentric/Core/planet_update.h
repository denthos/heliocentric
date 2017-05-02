#pragma once

#include "game_object_update.h"
#include "planet.h"

class PlanetUpdate : public GameObjectUpdate {
public:
	void apply(GameObject* obj);
};