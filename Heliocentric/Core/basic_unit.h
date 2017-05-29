/**
@file unit_sample.h
@brief A sample unit.
*/
#pragma once
#include "unit.h"

/**
A sample unit class inherited from unit. All combat data should be initialized
in this class.
*/
class BasicUnit : public Unit {
public:
	/**
	Constructor for basic unit class.
	@param player The player that built this unit.
	@param position This unit's position on the map when it's built.
	@param manager The manager for this unit
	*/
	BasicUnit(glm::vec3 position, Player* owner, UnitManager* manager, UnitType* type);

	BasicUnit(UID id, glm::vec3 position, Player* owner, UnitManager* manager, UnitType* type);
};