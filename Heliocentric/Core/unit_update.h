#pragma once

#include "game_object_update.h"
#include "unit.h"

class UnitUpdate : public GameObjectUpdate {
public:
	/**
	Constructor for a unit update.
	@param id UID of the unit to be updated.
	@param x x-axis of the new position of the unit.
	@param y y-axis of the new position of the unit.
	@param z z-axis of the new position of the unit.
	*/
	int health;
	UnitUpdate(UID, float, float, float);
	UnitUpdate(UID, int, float, float, float);
	void apply(GameObject* obj);
	//void apply(Unit* obj);
};