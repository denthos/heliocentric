#pragma once

#include "game_object_update.h"
#include "unit.h"

class UnitUpdate : public GameObjectUpdate {
public:
	void apply(GameObject* obj);
};