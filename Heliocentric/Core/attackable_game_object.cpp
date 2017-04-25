#include "attackable_game_object.h"

AttackableGameObject::AttackableGameObject(glm::vec3 position, Player* player, int att, int def, int range, int heal) : 
	GameObject(position, player), combatAttack(att), combatDefense(def), combatRange(range), health(heal) {};


AttackableGameObject::AttackableGameObject(UID id, glm::vec3 position, Player* player, int att, int def, int range, int heal) : 
	GameObject(id, position, player), combatAttack(att), combatDefense(def), combatRange(range), health(heal) {};

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