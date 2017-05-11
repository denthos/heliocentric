#include "attackable_game_object.h"
#include "logging.h"
#include <glm\gtc\matrix_transform.hpp>


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

void AttackableGameObject::set_health(int heal) {
	health = heal;
}

int AttackableGameObject::take_damage(int damage) {
	int total_damage = (int)((damage / this->combatDefense) * 50);
	this->health = this->health - total_damage;
	LOG_DEBUG("Unit " + std::to_string(this->getID()) + " took damage, health is now " + std::to_string(health));
	return this->health;
}

void AttackableGameObject::do_attack(AttackableGameObject * target)
{	
	if (target == this || target->player == this->player) {
		LOG_ERR("Cannot attack same player.");
		return;
	}

	bool target_is_dead = false, this_is_dead = false;
	LOG_DEBUG("Attacker position is " + std::to_string(this->get_position().x) + " " + std::to_string(this->get_position().y) + " " + std::to_string(this->get_position().z));
	LOG_DEBUG("Target position is " + std::to_string(target->get_position().x) + " " + std::to_string(target->get_position().y) + " " + std::to_string(target->get_position().z));
	LOG_DEBUG("Distance between attacker and target is " + std::to_string(glm::distance(this->position, target->get_position())));

	if (glm::distance(this->position, target->get_position()) <= (float) this->combatRange) {
		// Target is within range.

		target->take_damage(this->get_combat_attack());

		if (target->health <= 0) {
			target->handle_defeat(this);
			target_is_dead = true;
			LOG_DEBUG("Enemy is dead.");
		}
		else {
			target->handle_counter(this);
			LOG_DEBUG("Enemy is countering.");
		}

		if (this->health <= 0) {
			this->handle_defeat(target);
			this_is_dead = true;
			LOG_DEBUG("Player spaceship is dead.");
		}
		if (target_is_dead != this_is_dead) {
			// Someone is still alive -- they are the victor. 
			(target_is_dead ? this : target)->handle_victory((target_is_dead) ? target : this);
		}
	}
	else {
		// Target is out of range.
		this->handle_out_of_range(target);
		LOG_DEBUG("Target " + std::to_string(target->getID()) + " is out of range, moving towards " + std::to_string(target->get_position().x) + " " + std::to_string(target->get_position().y) + " " + std::to_string(target->get_position().z));
	}

}
