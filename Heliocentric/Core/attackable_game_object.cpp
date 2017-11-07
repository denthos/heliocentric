#include "attackable_game_object.h"
#include "logging.h"
#include <glm\gtc\matrix_transform.hpp>


AttackableGameObject::AttackableGameObject(glm::vec3 position, Player* player, Attack* attack, int def, int heal) : 
	GameObject(position, player),  attack(*attack), combatDefense(def),  health(heal) {};

AttackableGameObject::AttackableGameObject(UID id, glm::vec3 position, Player* player, Attack* attack, int def, int heal) : 
	GameObject(id, position, player), attack(*attack), combatDefense(def), health(heal) {};


bool AttackableGameObject::is_dead() {
	return this->get_health() <= 0;
}

Attack& AttackableGameObject::getAttack() {
	return this->attack;
}

int AttackableGameObject::get_combat_defense() {
	return this->combatDefense;
}

int AttackableGameObject::set_combat_defense(int combatDefense) {
	this->combatDefense = combatDefense;
	return this->combatDefense;
}

int AttackableGameObject::get_health() {
	return this->health;
}

void AttackableGameObject::set_health(int heal) {
	health = heal;
}

int AttackableGameObject::take_damage(int damage) {
	int total_damage = (int)((((float) damage) / this->combatDefense) * 50);
	this->health = this->health - total_damage;
	LOG_DEBUG("Unit " + std::to_string(this->getID()) + " took damage, health is now " + std::to_string(health));
	return this->health;
}

bool AttackableGameObject::do_attack(std::shared_ptr<AttackableGameObject> target)
{
	if (target == nullptr) {
		LOG_ERR("target is null.");
		return false;
	}
	else if (target->getID() == this->getID() || target->player == this->player) {
		LOG_ERR("Cannot attack same player.");
		return false;
	}
	else if (target->is_dead()) {
		LOG_ERR("Cannot attack a dead unit.");
		return false;
	}

	bool target_is_dead = false, this_is_dead = false;
	LOG_DEBUG("Attacker position is " + std::to_string(this->get_position().x) + " " + std::to_string(this->get_position().y) + " " + std::to_string(this->get_position().z));
	LOG_DEBUG("Target position is " + std::to_string(target->get_position().x) + " " + std::to_string(target->get_position().y) + " " + std::to_string(target->get_position().z));
	LOG_DEBUG("Distance between attacker and target is " + std::to_string(glm::distance(this->get_position(), target->get_position())));

	if (glm::distance(this->get_position(), target->get_position()) <= (float) this->attack.getRange()) {

		// Target is within range.
		this->attack.doAttack(target.get());

		if (target->is_dead()) {
			target->handle_defeat(this->shared_from_this());
			target_is_dead = true;
			LOG_DEBUG("Enemy is dead.");
		}
		else {
			target->handle_counter(this->shared_from_this());
			LOG_DEBUG("Enemy is countering.");
		}

		if (this->is_dead()) {
			this->handle_defeat(target);
			this_is_dead = true;
			LOG_DEBUG("Player spaceship is dead.");
		}
		if (target_is_dead != this_is_dead) {
			// Someone is still alive -- they are the victor. 
			(target_is_dead ? this->shared_from_this() : target)->handle_victory((target_is_dead) ? target : this->shared_from_this());
		}
	}
	else {
		// Target is out of range.
		this->handle_out_of_range(target);
		LOG_DEBUG("Target " + std::to_string(target->getID()) + " is out of range, moving towards " + std::to_string(target->get_position().x) + " " + std::to_string(target->get_position().y) + " " + std::to_string(target->get_position().z));
	}

	return true;

}
