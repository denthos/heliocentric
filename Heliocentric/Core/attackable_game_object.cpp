#include "attackable_game_object.h"


int AttackableGameObject::get_combat_attack() {
	return this->combatAttack;
}

int AttackableGameObject::set_combat_attack(int combatAttack) {
	this->combatAttack = combatAttack;
	return this->combatAttack;
}

int AttackableGameObject::get_combat_defense() {
	return this->combatDefense;
}

int AttackableGameObject::set_combat_defense(int combatDefense) {
	this->combatDefense = combatDefense;
	return this->combatDefense;
}

int AttackableGameObject::get_combat_range() {
	return this->combatRange;
}

int AttackableGameObject::set_combat_range(int combatRange) {
	this->combatRange = combatRange;
	return this->combatRange;
}

int AttackableGameObject::get_health() {
	return this->health;
}

int AttackableGameObject::take_damage(AttackableGameObject* attacker) {
	int damage = (int)((attacker->get_combat_attack() / this->combatDefense) * 50);
	this->health = this->health - damage;
	return this->health;
}

void AttackableGameObject::do_attack(AttackableGameObject * target)
{	
	Lib::assertTrue(target != this, "Attackable cannot attack itself.");
	Lib::assertTrue(target->player != this->player, "Player cannot attack their own attackables.");

	float distance = glm::distance(this->position, target->get_position());
	bool target_is_dead = false, this_is_dead = false;

	if (glm::distance(this->position, target->get_position()) <= (float) this->combatRange) {
		// Target is within range.

		target->take_damage(this);

		if (target->health <= 0) {
			target->handle_defeat(this);
			target_is_dead = true;
		}
		else {
			target->handle_counter(this);
		}

		if (this->health <= 0) {
			this->handle_defeat(target);
			this_is_dead = true;
		}
		if (target_is_dead != this_is_dead) {
			// Someone is still alive -- they are the victor. 
			(target_is_dead ? this : target)->handle_victory((target_is_dead) ? target : this);
		}
	}
	else {
		// Target is out of range.
		this->handle_out_of_range(target);
	}

}
