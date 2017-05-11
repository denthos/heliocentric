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
	@param orient_x x-axis of new orientation of the unit.
	@param orient_y y-axis of new orientation of the unit.
	@param orient_z z-axis of new orientation of the unit.
	*/
	int health;
	float orient_x;
	float orient_y;
	float orient_z;
	UnitUpdate(UID, float, float, float);
	UnitUpdate(UID, int, float, float, float);
	void apply(GameObject* obj);
};