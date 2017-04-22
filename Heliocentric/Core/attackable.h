/**
@file attackable.h
@brief An abstract class that defines an attackable object.
*/
#pragma once
#include "lib.h"

/**
An abstract class that defines an attackable object.
*/
class Attackable {
protected:
	int combatAttack;
	int combatDefense;
	int combatRange; // An attackable can attack its target only when target is in this range.
	int health;
	int tier;
	
public:
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

	/**
	Calculates damage inflicted on this unit and set subtract damage from this
	unit's health. Returns unit's health after taking damage. Unit is dead if
	remaining health becomes less than or equal to zero.
	@param attacker The attackable object that initiated the attack.
	@return Health of this unit after taking damage from the attack.
	*/
	int take_damage(Attackable* attacker);

	/**
	Allows to check the current tier.
	*/	
	int get_tier();

	/** 
	Update tier value.
	*/
	void upgrade_tier();
};