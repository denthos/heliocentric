/**
@file unit.h
@brief An abstract class that defines a base unit.
*/
#pragma once
#include "lib.h"
#include "attackable_game_object.h"

#include <memory>

class UnitUpdate;

/**
An abstract class that defines a base unit.
*/
class Unit : public AttackableGameObject {
public:

	friend UnitUpdate;

	Unit(glm::vec3 pos, Player* owner, int att, int def, int range, int heal);
	Unit(UID id, glm::vec3 pos, Player* owner, int att, int def, int range, int heal);

	/**
	Called continuously by server to update current state of the unit.
	*/
	void update_command();

	/** 
	Perform logic based on command
	*/
	void do_logic();

	/**
	Perform update
	*/
	std::shared_ptr<UnitUpdate> make_update();

	/**
	Returns the destination where the unit is moving towards.
	@return The destination where the unit is moving towards.
	*/
	glm::vec3 get_destination();

	/**
	Sets the destination where the unit is moving towards.
	@param destination The destination where the unit is moving towards.
	@return The destination of this unit.
	*/
	glm::vec3 set_destination(glm::vec3 destination);

	/**
	Returns the maximum movement speed of this unit.
	@return The maximum movement speed of this unit.
	*/
	int get_movement_speed_max();

	/**
	Sets the maximum movement speed of this unit.
	@param The maximum movement speed of this unit.
	@return The maximum movement speed of this unit.
	*/
	int set_movmennt_speed_max(int movementSpeedMax);

protected:
	/**
	Tells the unit to attack the target.
	@param target The target that this unit is attacking.
	@return Target of this unit.
	*/
	Unit* do_attack(Unit* target);

	/**
	Tells the unit to move towards destination.
	@return Current position of this unit.
	*/
	glm::vec3 do_move();

	enum CommandType { attack, idle, move };

	int movementSpeedMax; // maximum speed that this unit can achieve when powered by its own engine
	int movementSpeedCurrent; // can be used if implementing gravity simulation
	CommandType currentCommand;
	glm::vec3 destination;
	std::shared_ptr<UnitUpdate> update;
};
