#include "attackable.h"

int Attackable::get_combat_attack() {
	return this->combatAttack;
}

int Attackable::set_combat_attack(int combatAttack) {
	this->combatAttack = combatAttack;
	return this->combatAttack;
}

int Attackable::get_combat_defense() {
	return this->combatDefense;
}

int Attackable::set_combat_defense(int combatDefense) {
	this->combatDefense = combatDefense;
	return this->combatDefense;
}

int Attackable::get_combat_range() {
	return this->combatRange;
}

int Attackable::set_combat_range(int combatRange) {
	this->combatRange = combatRange;
	return this->combatRange;
}

int Attackable::get_health() {
	return this->health;
}

int Attackable::take_damage(Attackable* attacker) {
	int damage = (int)((attacker->get_combat_attack() / this->combatDefense) * 50);
	this->health = this->health - damage;
	return this->health;
}

int Attackable::get_tier() {
	return tier;
}

void Attackable::upgrade_tier() {
	++tier;
}