/**
@file attackable_game_object.h
@brief An abstract class that defines an attackable game object.
*/
#pragma once
#include "lib.h"
#include "game_object.h"

/**
An abstract class that defines an attackable game object.
*/
class AttackableGameObject : public GameObject {
protected:
	int combatAttack;
	int combatDefense;
	int combatRange; // An attackable game object can attack its target only when target is in this range.
	int health;


	/** Combat Hooks **/

	/**
	Hook that allows defender to react after being attacked.
	@param The attacking opponent
	*/
	virtual void handle_counter(AttackableGameObject* attacker) = 0;

	/**
	Hook for handling when an initiated attack fails because the opponent
	is out of range. 
	@param The combat opponent.
	*/
	virtual void handle_out_of_range(AttackableGameObject* opponent) = 0;

	/**
	Hook for handling when the attackable's health drops below zero.
	@param The defeating opponent.
	*/
	virtual void handle_defeat(AttackableGameObject* opponent) = 0;

	/**
	Hook for handling an opponent's defeat. 
	@param The defeated opponent.
	*/
	virtual void handle_victory(AttackableGameObject* opponent) = 0;


public:


	/**
	Creates an attackable 
	*/
	AttackableGameObject(glm::vec3 position, Player* player, int att, int def, int range, int heal);


	AttackableGameObject(UID id, glm::vec3 position, Player* player, int att, int def, int range, int heal);

	/**
	Returns the attack strength of this object.
	@return The attack strength of this object.
	*/
	int get_combat_attack();

	/**
	Sets the attack strength of this object.
	@param combatAttack The attack strength of this object.
	@return The new attack strength of this object.
	*/
	int set_combat_attack(int combatAttack);

	/**
	Returns the defense strength of this object.
	@return The defense strength of this object.
	*/
	int get_combat_defense();

	/**
	Sets the defense strength of this object.
	@param combatDefense The defense strength of this object.
	@return The new defense strength of this object.
	*/
	int set_combat_defense(int combatDefense);

	/**
	Returns the combat range of this object.
	@return The combat range of this object.
	*/
	int get_combat_range();

	/**
	Sets the combat range of this object.
	@param combatRange The combat range of this object.
	@return The new combat range of this object.
	*/
	int set_combat_range(int combatRange);

	/**
	Returns current health of this object.
	@return Current health of this object.
	*/
	int get_health();

	void set_health(int heal);

	/**
	Subtract damage from this unit's health. Returns unit's health after taking 
	damage. Unit is dead if remaining health becomes less than or equal to zero.
	@param attacker The attackable object that initiated the attack.
	@return Health of this unit after taking damage from the attack.
	*/
	int take_damage(int damage);


	/**
	Performs combat logic for attackable game objects. For implementing interesting 
	combat logic, the handle_* hooks are available for subclasses to override. 
	@param The target to attack.
	*/
	void do_attack(AttackableGameObject* target);
};

