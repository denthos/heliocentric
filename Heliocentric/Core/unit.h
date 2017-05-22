/**
@file unit.h
@brief An abstract class that defines a base unit.
*/
#pragma once
#include "lib.h"
#include "attackable_game_object.h"

#include <memory>

class UnitUpdate;
class UnitManager;

/**
An abstract class that defines a base unit.
*/
class Unit : public AttackableGameObject {
public:

	friend UnitUpdate;

	enum CommandType { UNIT_ATTACK, UNIT_IDLE, UNIT_MOVE, UNIT_DIE };

	/**
	Unit constructor without specifying UID. Used on server end.
	@param pos Position of the unit.
	@param owner Indicates which player owns this unit.
	@param att Attack stat of this unit.
	@param def Defense stat of this unit.
	@param range Range stat of this unit.
	@param heal Health stat of this unit.
	@param movement_speed Movement speed of this unit.
	*/
	Unit(glm::vec3 pos, Player* owner, Attack* attack, int def, int heal, float movement_speed = 1.0f);

	/**
	Unit constructor with a specified UID. Essentially creates a "copy" of this
	unit, since it does not create a new UID for it. Used on client end.
	@param id UID of this unit.
	@param pos Position of the unit.
	@param owner Indicates which player owns this unit.
	@param att Attack stat of this unit.
	@param def Defense stat of this unit.
	@param range Range stat of this unit.
	@param heal Health stat of this unit.
	@param movement_speed Movement speed of this unit.
	*/
	Unit(UID id, glm::vec3 pos, Player* owner, Attack* attack, int def, int heal, float movement_speed = 1.0f);
  
	/** 
	Perform logic based on command. Called continuously by server to update current state 
	of the unit.
	*/
	CommandType do_logic();

	/**
	Perform update.
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
	Sets the unit to follow the given game object.
	@param The game object which this unit should follow.
	@return The destination of this unit.
	*/
	glm::vec3 set_destination(GameObject* object);

	/**
	Returns the maximum movement speed of this unit.
	@return The maximum movement speed of this unit.
	*/
	float get_movement_speed_max();

	/**
	Sets the maximum movement speed of this unit.
	@param The maximum movement speed of this unit.
	@return The maximum movement speed of this unit.
	*/
	void set_movement_speed_max(float movement_speed);


	/**
	Tells the unit to attack the target.
	@param The target that this unit is attacking.
	*/
	void set_combat_target(AttackableGameObject* target);


	/**
	Sets the command state of the unit
	**/
	void set_command(CommandType command);


	/**
	Sets the unit's UnitManager.
	**/
	void set_manager(UnitManager* manager);

	bool isMoving();
protected:

	/**
	Tells the unit to move towards destination.
	@return Current position of this unit.
	*/
	glm::vec3 do_move();

	float movement_speed;  // maximum speed that this unit can achieve when powered by its own engine

	std::shared_ptr<UnitUpdate> update;
	CommandType currentCommand = UNIT_IDLE;
	glm::vec3 destination;

	virtual void handle_out_of_range(AttackableGameObject* opponent);
	virtual void handle_defeat(AttackableGameObject* opponent);
	virtual void handle_victory(AttackableGameObject* opponent);
	virtual void handle_counter(AttackableGameObject* opponent);

	void send_update_to_manager(std::shared_ptr<UnitUpdate>& update);

	// TODO: Change these to smart pointers.
	AttackableGameObject* target;
	UnitManager* manager;
	bool moving = false;
};
