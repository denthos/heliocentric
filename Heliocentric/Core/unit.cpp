#include "unit.h"
#include "logging.h"
#include "unit_update.h"
#include "glm/gtc/matrix_transform.hpp"

Unit::Unit(glm::vec3 pos, Player* owner, int att, int def, int range, int heal, float movement_speed) :
	AttackableGameObject(pos, owner, att, def, range, heal), orientation(glm::vec3(0.0f, 0.0f, 1.0f)) {
	this->update = std::make_shared<UnitUpdate>(this->getID(), this->get_health(), pos.x, pos.y, pos.z);
	this->movement_speed = movement_speed;
	this->delta_time_for_orient = 0.0f;
}

Unit::Unit(UID id, glm::vec3 pos, Player* owner, int att, int def, int range, int heal, float movement_speed) :
	AttackableGameObject(id, pos, owner, att, def, range, heal), orientation(glm::vec3(0.0f, 0.0f, 1.0f)) {
	this->update = std::make_shared<UnitUpdate>(id, this->get_health(), pos.x, pos.y, pos.z);
	this->movement_speed = movement_speed;
	this->delta_time_for_orient = 0.0f;
}

Unit::CommandType Unit::do_logic() {
	switch (currentCommand) {
	case UNIT_IDLE:
		break;
	case UNIT_ATTACK:
		do_attack(this->target);
		break;
	case UNIT_MOVE:
		do_move();
		break;
	case UNIT_DIE:
		break;
	case UNIT_HOLDER:
		break;
	default:
		LOG_ERR("Invalid command type.");
	}
	return currentCommand;
}


std::shared_ptr<UnitUpdate> Unit::make_update() {
	this->update->id = this->getID();
	this->update->health = this->get_health();
	this->update->x = this->position.x;
	this->update->y = this->position.y;
	this->update->z = this->position.z;
	this->update->orient_x = this->orientation.x;
	this->update->orient_y = this->orientation.y;
	this->update->orient_z = this->orientation.z;
	this->update->rot_mat = this->rotation;
	LOG_DEBUG("Unit with ID " + std::to_string(this->update->id) + " with health " + std::to_string(this->update->health) +  ". Position is " + std::to_string(this->update->x) + " " + std::to_string(this->update->y) + " " + std::to_string(this->update->z) );
	LOG_DEBUG("Orientation is " + std::to_string(this->update->orient_x) + " " + std::to_string(this->update->orient_y) + " " + std::to_string(this->update->orient_z));
	return this->update;
};

glm::vec3 Unit::get_destination() {
	return this->destination;
}

glm::vec3 Unit::set_destination(glm::vec3 destination) {
	this->destination = destination;
	return this->destination;
}

void Unit::set_orientation(glm::vec3 new_orientation) {
	orientation = new_orientation;
}

glm::vec3 Unit::get_orientation() {
	return orientation;
}

glm::vec3 Unit::set_destination(GameObject* object) {
	// Follow object as it moves.
	this->destination = object->get_position();
	return this->destination;
}


float Unit::get_movement_speed_max() {
	return this->movement_speed;
}

void Unit::set_movement_speed_max(float movement_speed) {
	this->movement_speed = movement_speed;
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
		float speed = fmin(movement_speed, glm::distance(destination, position));
		position += glm::normalize(destination - position) * speed;
	}
	else {
		// Reached destination
		currentCommand = UNIT_IDLE;
	}
	return position;
}

void Unit::do_orient() {
	// Orient towards destination
	if ((destination - position) == glm::vec3(0.0)) {
		delta_time_for_orient = 0.0f;

		return;
	
	}
	glm::vec3 ideal_orientation = glm::normalize(destination - position);
	glm::vec3 delta_orientation = glm::mix(orientation, ideal_orientation, delta_time_for_orient);
	orientation = glm::normalize(delta_orientation);

	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 xaxis = glm::cross(up, orientation);
	xaxis = glm::normalize(xaxis);
	glm::vec3 yaxis = glm::cross(orientation, xaxis);
	yaxis = glm::normalize(yaxis);

	rotation[0][0] = xaxis.x;
	rotation[0][1] = yaxis.x;
	rotation[0][2] = orientation.x;

	rotation[1][0] = xaxis.y;
	rotation[1][1] = yaxis.y;
	rotation[1][2] = orientation.y;

	rotation[2][0] = xaxis.z;
	rotation[2][1] = yaxis.z;
	rotation[2][2] = orientation.z;

	LOG_DEBUG("Unit with ID: " + std::to_string(this->getID()) + " is orienting: ");
	LOG_DEBUG("Ideal Orientation is " + std::to_string(ideal_orientation.x) + " " + std::to_string(ideal_orientation.y) + " " + std::to_string(ideal_orientation.z));
	LOG_DEBUG("New Orientation is " + std::to_string(orientation.x) + " " + std::to_string(orientation.y) + " " + std::to_string(orientation.z));
	LOG_DEBUG("Delta time is " + std::to_string(delta_time_for_orient));
	if (delta_time_for_orient >= 1.0f)
		delta_time_for_orient = 0.0f;
	else
		delta_time_for_orient += 0.1f;
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
	//player->add_to_destroy(this);
	//player = nullptr;
	this->set_command(Unit::UNIT_DIE);
	LOG_DEBUG("Unit " + std::to_string(this->getID()) + " set to UNIT_DIE.");
}

void Unit::handle_victory(AttackableGameObject * opponent)
{
	// Go back to idle if unit was attacking
	currentCommand = (currentCommand == UNIT_ATTACK) ? UNIT_IDLE : currentCommand;
	
	// TODO: Gain experience (?)
}

void Unit::do_attack(AttackableGameObject* target) {
	float distance = glm::distance(this->position, target->get_position());
	float dot_product = glm::dot(glm::normalize(destination - position), glm::normalize(orientation));
	if (distance <= (float) this->combatRange && (dot_product < 1.05f && dot_product > 0.95f)) {
		AttackableGameObject::do_attack(target);
		return;
	}
	if (dot_product >= 1.05f || dot_product <= 0.95f) {
		LOG_DEBUG("dot product is " + std::to_string(dot_product));
		do_orient();
	}
	if (distance > (float) this->combatRange) {
		handle_out_of_range(target);
	}
}

