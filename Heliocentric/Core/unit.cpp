#include "unit.h"
#include "logging.h"
#include "unit_update.h"
#include "unit_manager.h"
//#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform.hpp>
#include "glm\gtx\quaternion.hpp"

Unit::Unit(glm::vec3 pos) : AttackableGameObject(pos), orientation(glm::vec3(0.0f, 0.0f, 1.0f)), rotation(glm::mat4(1.0f)) {
	this->manager = nullptr;
	this->target = nullptr;
	this->delta_time_for_orient = 0.0f;
}

Unit::Unit(glm::vec3 pos, Player* owner, Attack* attack, int def, int heal, float movement_speed) :
	AttackableGameObject(pos, owner, attack, def, heal), orientation(glm::vec3(0.0f, 0.0f, 1.0f)), rotation(glm::mat4(1.0f)) {
	this->update = std::make_shared<UnitUpdate>(this->getID(), this->get_health(), pos.x, pos.y, pos.z);
	this->movement_speed = movement_speed;
	this->manager = nullptr;
	this->target = nullptr;
	this->delta_time_for_orient = 0.0f;
}

Unit::Unit(UID id, glm::vec3 pos, Player* owner, Attack* attack, int def, int heal, float movement_speed) :
	AttackableGameObject(id, pos, owner, attack, def, heal), orientation(glm::vec3(0.0f, 0.0f, 1.0f)), rotation(glm::mat4(1.0f)) {
	this->update = std::make_shared<UnitUpdate>(id, this->get_health(), pos.x, pos.y, pos.z);
	this->movement_speed = movement_speed;
	this->delta_time_for_orient = 0.0f;
	this->manager = nullptr;
	this->target = nullptr;
}

Unit::CommandType Unit::do_logic() {
	switch (currentCommand) {
	case UNIT_IDLE:
		break;
	case UNIT_ATTACK:
		if (!do_attack(this->target)) {
			currentCommand = UNIT_IDLE;
		}
		break;
	case UNIT_MOVE:
		do_move();
		break;
	case UNIT_DIE:
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
	this->update->shoot_laser = this->get_laser_shooting();
	this->update->explode = this->explode;
	//LOG_DEBUG("Unit with ID " + std::to_string(this->update->id) + " with health " + std::to_string(this->update->health) +  ". Position is " + std::to_string(this->update->x) + " " + std::to_string(this->update->y) + " " + std::to_string(this->update->z) );
	//LOG_DEBUG("Orientation is " + std::to_string(this->update->orient_x) + " " + std::to_string(this->update->orient_y) + " " + std::to_string(this->update->orient_z));
	//LOG_DEBUG("Unit shoot laser is set to " + std::to_string(this->update->shoot_laser));
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

	if (currentCommand != UNIT_DIE)
	{
		currentCommand = command;
	}
}

void Unit::set_laser_shooting(bool shoot) {
	this->shoot_laser = shoot;
}

bool Unit::get_laser_shooting() {
	return this->shoot_laser;
}

void Unit::set_explode(bool explode) {
	this->explode = explode;
}

bool Unit::get_explode() {
	return this->explode;
}

glm::vec3 Unit::do_move() {
	// Move towards destination.
	if (destination != position) {
		float speed = fmin(movement_speed, glm::distance(destination, position));
		position += glm::normalize(destination - position) * speed;
		send_update_to_manager(make_update());
	}
	else {
		// Reached destination
		currentCommand = UNIT_IDLE;
	}
	return position;
}

void Unit::do_orient() {
	glm::vec3 ideal_orientation = destination - position;
	ideal_orientation.y = 0.0f;
	ideal_orientation = glm::normalize(ideal_orientation);

	// Orient towards destination
	if (ideal_orientation == glm::vec3(0.0)) {
		delta_time_for_orient = 0.0f;
		return;
	}
	else if (delta_time_for_orient > 0.0f) {
		glm::quat toRotQuat = glm::quat_cast((lookAt)); //the final rotation quaternion
		glm::quat rotQuat = glm::slerp(glm::quat_cast(old_rotation), toRotQuat, delta_time_for_orient);
		rotation = glm::mat4_cast(rotQuat);
	}
	else {
		old_rotation = rotation;
		LOG_DEBUG("Destination is " + std::to_string(destination.x) + " " + std::to_string(destination.y) + " " + std::to_string(destination.z));
		LOG_DEBUG("Position is " + std::to_string(position.x) + " " + std::to_string(position.y) + " " + std::to_string(position.z));
		LOG_DEBUG("Ideal orientation is " + std::to_string(ideal_orientation.x) + " " + std::to_string(ideal_orientation.y) + " " + std::to_string(ideal_orientation.z));
		LOG_DEBUG("Current orientation is " + std::to_string(orientation.x) + " " + std::to_string(orientation.y) + " " + std::to_string(orientation.z));
		float cosine = glm::clamp(glm::dot(orientation, ideal_orientation), -1.0f, 1.0f);
		float rotAngle = glm::acos(cosine / (glm::length(orientation) * glm::length(ideal_orientation)));
		glm::vec3 cross = glm::cross(orientation, ideal_orientation);
		if (cross.y < 0) {
			rotAngle = -rotAngle;
		}
		//LOG_DEBUG("dot product is " + std::to_string(glm4::dot(orientation, ideal_orientation)));
		LOG_DEBUG("Cross product is " + std::to_string(cross.x) + " " + std::to_string(cross.y) + " " + std::to_string(cross.z));
		LOG_DEBUG("Rotation angle in radian is " + std::to_string(rotAngle));
		LOG_DEBUG("Rotation angle in degree is " + std::to_string(glm::degrees(rotAngle)));
		lookAt = glm::rotate(rotation, rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::quat toRotQuat = glm::quat_cast((lookAt)); //the final rotation quaternion
		//rotation = lookAt;
	}

	LOG_DEBUG("Unit with ID: " + std::to_string(this->getID()) + " is orienting: ");
	LOG_DEBUG("Delta time is " + std::to_string(delta_time_for_orient));
	if (delta_time_for_orient >= 1.0f) {
		delta_time_for_orient = 0.0f;
		orientation = ideal_orientation;
	}
	else {
		delta_time_for_orient += 0.1f;
	}
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
	send_update_to_manager(make_update());
	this->set_command(Unit::UNIT_DIE);
	LOG_DEBUG("Unit " + std::to_string(this->getID()) + " set to UNIT_DIE.");
	
}

void Unit::handle_victory(AttackableGameObject * opponent)
{
	// Go back to idle if unit was attacking
	currentCommand = (currentCommand == UNIT_ATTACK) ? UNIT_IDLE : currentCommand;
	this->set_laser_shooting(false);
	// TODO: Gain experience (?)
}

bool Unit::do_attack(AttackableGameObject* target) {
	float distance = glm::distance(this->position, target->get_position());
	glm::vec3 ideal_orient = destination - position;
	ideal_orient.y = 0.0f;
	ideal_orient = glm::normalize(ideal_orient);
	float dot_product = glm::dot(ideal_orient, glm::normalize(orientation));
	LOG_DEBUG("dot product is " + std::to_string(dot_product));


	if ((distance <= (float) this->attack.getRange() && Lib::floatCompare(dot_product, 1.0f)) || distance == 0.0f) {
		set_laser_shooting(true);
		return AttackableGameObject::do_attack(target);
	}
	if (!Lib::floatCompare(dot_product, 1.0f)) {
		do_orient();
	}
	if (distance > (float) this->attack.getRange()) {
		handle_out_of_range(target);
	}
	return true;
}

void Unit::handle_counter(AttackableGameObject* opponent) {
	// Unit has been attacked, notify 
	send_update_to_manager(make_update());
}


void Unit::set_manager(UnitManager* manager) {
	this->manager = manager;
}


void Unit::send_update_to_manager(std::shared_ptr<UnitUpdate>& update) {
	if (this->manager != nullptr) {
		this->manager->register_update(update);
	}
}
