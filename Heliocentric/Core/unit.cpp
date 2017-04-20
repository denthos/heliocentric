#include "unit.h"

void Unit::update() {
	switch (currentCommand) {
	case idle:
		break;
	case attack:
		// Placeholder parameter. Cannot quite get target yet, as "delta" of a unit has not been implemented.
		do_attack(this);
		break;
	case move:
		do_move();
		break;
	default:
		Lib::debug("Invalid command type.");
	}
}

glm::vec3 Unit::get_destination() {
	return this->destination;
}

glm::vec3 Unit::set_destination(glm::vec3 destination) {
	this->destination = destination;
	return this->destination;
}

int Unit::get_movement_speed_max() {
	return this->movementSpeedMax;
}

int Unit::set_movmennt_speed_max(int movementSpeedMax) {
	this->movementSpeedMax = movementSpeedMax;
	return this->movementSpeedMax;
}

Unit* Unit::do_attack(Unit* target) {
	Lib::assertTrue(target != this, "Unit attacking itself.");
	if (glm::distance(this->position, target->get_position()) > (float) this->combatRange) {
		do_move();
	}
	else {
		int targetHealth = target->take_damage(this);
		// Reset unit to idle if target is dead.
		if (targetHealth <= 0) this->currentCommand = idle;
	}

	return target;
}

glm::vec3 Unit::do_move() {
	return this->position;
}