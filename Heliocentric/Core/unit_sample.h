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
class UnitSample : Unit {
public:
	/**
	Constructor for sample unit class.
	@param player The player that built this unit.
	@param position This unit's position on the map when it's built.
	*/
	UnitSample(Player* player, glm::vec3 position);
};