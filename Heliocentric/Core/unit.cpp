#include "unit.h"
#include "logging.h"

Unit::Unit(glm::vec3 pos, Player* owner, int att, int def, int range, int heal):
	AttackableGameObject(pos, owner, att, def, range, heal) {}

Unit::Unit(UID id, glm::vec3 pos, Player* owner, int att, int def, int range, int heal):
	AttackableGameObject(id, pos, owner, att, def, range, heal) {}

void Unit::update() {
	switch (currentCommand) {
	case UNIT_IDLE:
		break;
	case UNIT_ATTACK:
		do_attack(this->target);
		break;
	case UNIT_MOVE:
		do_move();
		break;
	default:
		Lib::LOG_ERR("Invalid command type.");
	}
}

glm::vec3 Unit::get_destination() {
	return this->destination;
}

glm::vec3 Unit::set_destination(glm::vec3 destination) {
	this->destination = destination;
	return this->destination;
}


glm::vec3 Unit::set_destination(GameObject* object) {
	// Follow object as it moves.
	this->destination = object->get_position();
	return this->destination;
}


int Unit::get_movement_speed_max() {
	return this->movementSpeedMax;
}

int Unit::set_movmennt_speed_max(int movementSpeedMax) {
	this->movementSpeedMax = movementSpeedMax;
	return this->movementSpeedMax;
}

void Unit::set_combat_target(AttackableGameObject* target) {
	this->target = target;
}

void Unit::set_command(CommandType command) {
	currentCommand = command;
}


glm::vec3 Unit::do_move() {
	// Move towards destination.
	if (destination != position) {
		float speed = fmin((float)movementSpeedMax, glm::distance(destination, position));
		position += glm::normalize(destination - position) * speed;
	}
	else {
		// Reaced destination
		currentCommand = UNIT_IDLE;
	}
	return position;
}

void Unit::handle_out_of_range(AttackableGameObject * opponent)
{
	// Move towards opponent if in attack mode.
	if (currentCommand == UNIT_ATTACK) {
		set_destination(opponent);
		do_move();
	}
}

void Unit::handle_defeat(AttackableGameObject * opponent)
{
	// Tell Player you have died.
	player->add_to_destroy(this);
	player = nullptr;
}

void Unit::handle_victory(AttackableGameObject * opponent)
{
	// Go back to idle if unit was attacking
	currentCommand = (currentCommand == UNIT_ATTACK) ? UNIT_IDLE : currentCommand;
	
	// TODO: Gain experience (?)
}

