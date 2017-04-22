#include "attackable_game_object.h"

bool AttackableGameObject::attack_helper(AttackableGameObject * target)
{
	if (glm::distance(this->position, target->get_position()) > (float) this->combatRange) {
		handle_out_of_range(target);
	}
	else {
		target->take_damage(this);
	}

	if (target->health <= 0) {
		target->handle_defeat(target);
		return true;
	}
	return false;
}

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
	bool target_dead = attack_helper(target), aggressor_dead = target->attack_helper(this);
	if (target_dead != aggressor_dead) {
		(target_dead ? this : target)->handle_victory(target_dead ? target : this);
	}
}
