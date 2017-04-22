#include "unit.h"

void Unit::update() {
	switch (currentCommand) {
	case idle:
		break;
	case attack:
		do_attack(this->target);
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

void Unit::set_attack_target(AttackableGameObject* target) {
	this->target = target;
	currentCommand = attack;
}

glm::vec3 Unit::do_move() {
	// Move towards destination.
	return position + (glm::normalize(destination - position) * (float) movementSpeedCurrent);
}

void Unit::handle_out_of_range(AttackableGameObject * opponent)
{
	// Move towards opponent if in attack mode.
	if (currentCommand == attack) {
		set_destination(opponent->get_position());
		do_move();
	}
}

void Unit::handle_defeat(AttackableGameObject * opponent)
{
	// Tell Player you have died.
	player->add_to_destroy(this);
}
