#include "unit.h"
#include "logging.h"
#include "unit_update.h"
#include "unit_manager.h"
#include "glm/gtc/matrix_transform.hpp"

Unit::Unit(glm::vec3 pos, Player* owner, Attack* attack, int def, int heal, float movement_speed) :
	AttackableGameObject(pos, owner, attack, def, heal) {
	this->update = std::make_shared<UnitUpdate>(this->getID(), this->get_health(), pos.x, pos.y, pos.z);
	this->movement_speed = movement_speed;
	this->manager = nullptr;
	this->target = nullptr;
}

Unit::Unit(UID id, glm::vec3 pos, Player* owner, Attack* attack, int def, int heal, float movement_speed) :
	AttackableGameObject(id, pos, owner, attack, def, heal) {
	this->update = std::make_shared<UnitUpdate>(id, this->get_health(), pos.x, pos.y, pos.z);
	this->movement_speed = movement_speed;
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
	//this->update->moving = this->moving;
	LOG_DEBUG("Unit with ID " + std::to_string(this->update->id) + " with health " + std::to_string(this->update->health) +  ". Position is " + std::to_string(this->update->x) + " " + std::to_string(this->update->y) + " " + std::to_string(this->update->z) );
	return this->update;
};

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

int Unit::get_command() {

	return currentCommand;
}

bool Unit::isMoving() {
	return moving;
}

void Unit::set_moving(bool moving) {
	this->moving = moving;
}

glm::vec3 Unit::do_move() {
	// Move towards destination.
	if (destination != position) {
		float speed = fmin(movement_speed, glm::distance(destination, position));

		position += glm::normalize(destination - position) * speed;
		moving = true;
		send_update_to_manager(make_update());
		
	}
	else {
		// Reaced destination
		currentCommand = UNIT_IDLE;
		moving = false;
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
	
	// TODO: Gain experience (?)
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
