#pragma once

#include "game_object_update.h"
#include "unit_type.h"

class UnitCreationUpdate : public GameObjectUpdate {
public:
	UID player_id;
	int def, health;
	UnitType::TypeIdentifier type;

	/**
	Constructor for a UnitCreationUpdate, used when a unit is created.
	@param id UID of the unit to be created.
	@param x x-axis of the position where the unit is created.
	@param y y-axis of the position where the unit is created.
	@param z z-axis of the position where the unit is created.
	@param player_id UID of the player that owns this unit.
	@param def Defense of this unit.
	@param health Health of this unit.
	*/
	UnitCreationUpdate(UID, float, float, float, UID, int, int, UnitType::TypeIdentifier type);
	void apply(GameObject* obj);
};