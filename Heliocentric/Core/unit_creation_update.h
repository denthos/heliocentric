#pragma once

#include "game_object_update.h"
#include "unit.h"

class UnitCreationUpdate : public GameObjectUpdate {
public:
	UID player_id;
	int att, def, range, health;
	float movement_speed;

	/**
	Constructor for a UnitCreationUpdate, used when a unit is created.
	@param id UID of the unit to be created.
	@param x x-axis of the position where the unit is created.
	@param y y-axis of the position where the unit is created.
	@param z z-axis of the position where the unit is created.
	@param player_id UID of the player that owns this unit.
	@param att Attack of this unit.
	@param def Defense of this unit.
	@param range Attack range of this unit.
	@param health Health of this unit.
	@param movement_speed Maximum movement speed of this unit.
	*/
	UnitCreationUpdate(UID, float, float, float, UID, int, int, int, int, float);
	void apply(GameObject* obj);
};