#pragma once

#include "game_object_update.h"
#include "unit.h"

class UnitCreationUpdate : public GameObjectUpdate {
public:
	UID player_id;
	int att, def, range, health;

	void apply(GameObject* obj);
};