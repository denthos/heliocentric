#include "unit.h"
#include "logging.h"
#include "unit_update.h"

Unit::Unit(glm::vec3 pos, Player* owner, int att, int def, int range, int heal):
	AttackableGameObject(pos, owner, att, def, range, heal) {
	this->update = std::make_shared<UnitUpdate>();
}

Unit::Unit(UID id, glm::vec3 pos, Player* owner, int att, int def, int range, int heal):
	AttackableGameObject(id, pos, owner, att, def, range, heal) {
	this->update = std::make_shared<UnitUpdate>();
}

void Unit::update_command() {
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
		Lib::LOG_ERR("Invalid command type.");
	}
}

void Unit::do_logic() {
	glm::vec3 new_pos = this->get_position() + glm::vec3(0.01f, 0.01f, 0.0f);
	this->position = new_pos;
}

std::shared_ptr<UnitUpdate> Unit::make_update() {
	this->update->id = this->getID();
	this->update->x = this->position.x;
	this->update->y = this->position.y;
	this->update->z = this->position.z;
	Lib::LOG_DEBUG("Position is " + std::to_string(this->update->x) + " " + std::to_string(this->update->y) + " " + std::to_string(this->update->z) );
	return this->update;
};

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