#include "unit.h"
#include "logging.h"
#include "unit_update.h"
#include "unit_manager.h"
#include "attack.h"
#include "glm/gtc/matrix_transform.hpp"

Unit::Unit(glm::vec3 pos, Player* owner, Attack* attack, UnitManager* manager, int def, int heal, float movement_speed, const UnitType* type) :
	AttackableGameObject(pos, owner, attack, def, heal), manager(manager), target(nullptr), movement_speed(movement_speed), type(type), orientation(0.0f, 0.0f, 1.0f) {

	initialize();

}

Unit::Unit(UID id, glm::vec3 pos, Player* owner, Attack* attack, UnitManager* manager, int def, int heal, float movement_speed, const UnitType* type) :
	AttackableGameObject(id, pos, owner, attack, def, heal), manager(manager), target(nullptr), movement_speed(movement_speed), type(type), orientation(0.0f, 0.0f, 1.0f) {

	initialize();

}

const UnitType* Unit::getType() const {
	return this->type;
}

void Unit::initialize() {
	glm::vec3 pos = get_position();
	this->update = std::make_shared<UnitUpdate>(getID(), this->get_health(), pos.x, pos.y, pos.z);
}

Unit::CommandType Unit::do_logic() {
	switch (currentCommand) {
	case UNIT_IDLE:
		break;
	case UNIT_ATTACK:
		if (!do_attack(this->target)) {
			this->target.reset();
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

	this->update->orientation_x = this->orientation.x;
	this->update->orientation_y = this->orientation.y;
	this->update->orientation_z = this->orientation.z;
	// LOG_DEBUG("Unit with ID " + std::to_string(this->update->id) + " with health " + std::to_string(this->update->health) +  ". Position is " + std::to_string(this->update->x) + " " + std::to_string(this->update->y) + " " + std::to_string(this->update->z) );

	if (this->attack.projectileInMotion() || this->attack.damaging()) {
		this->update->attacking = true;
	}
	else {
		this->update->attacking = false;
	}

	return this->update;
};

glm::vec3 Unit::get_destination() {
	if (hasHighPri) {
		return hi_pri_dest;
	}
	return low_pri_dest;
}

void Unit::set_destination(glm::vec3 destination, bool high_pri) {
	if (high_pri) {
		this->hi_pri_dest = destination;
		hasHighPri = true;
	}
	else {
		this->low_pri_dest = destination;
	}
}


glm::vec3 Unit::set_destination(GameObject* object) {
	// Follow object as it moves.
	this->low_pri_dest = object->get_position();
	return this->low_pri_dest;
}

void Unit::do_orient(glm::vec3 target) {
	glm::vec3 ideal_orientation = target - this->position;
	ideal_orientation.y = 0;

	this->orientation = glm::normalize(ideal_orientation);
}

glm::vec3 Unit::get_orientation() const {
	return this->orientation;
}

void Unit::set_orientation(glm::vec3 orient) {
	this->orientation = orient;
}


float Unit::get_movement_speed_max() {
	return this->movement_speed;
}

void Unit::set_movement_speed_max(float movement_speed) {
	this->movement_speed = movement_speed;
}

void Unit::set_combat_target(std::shared_ptr<AttackableGameObject> target) {
	this->target.reset();
	this->target = target;
}

void Unit::set_command(CommandType command) {

	if (currentCommand != UNIT_DIE)
	{
		currentCommand = command;
	}
}


bool Unit::client_isAttacking() const {
	return this->client_isattacking;
}

void Unit::client_setAttacking(bool attacking) {
	this->client_isattacking = attacking;
}

bool Unit::do_attack(std::shared_ptr<AttackableGameObject> target) {
	do_orient(target->get_position());
	bool attack_return = AttackableGameObject::do_attack(target);
	send_update_to_manager(make_update());
	return attack_return;
}


glm::vec3 Unit::do_move() {
	// Move towards destination.
	glm::vec3 destination = hasHighPri ? hi_pri_dest : low_pri_dest;

	do_orient(destination);

	float dist_to_dest = glm::distance(position, destination);
	if (destination == glm::vec3(0.0f) && dist_to_dest < 100.0f) { //janky sun fix
		currentCommand = UNIT_IDLE;
		hasHighPri = false;
		return position;
	}

	if (destination != position) {
		float speed = fmin(movement_speed, glm::distance(destination, position));
		position += glm::normalize(destination - position) * speed;

		send_update_to_manager(make_update());
	}
	else {
		// Reached destination
		currentCommand = UNIT_IDLE;
	}

	hasHighPri = false;
	return position;
}

void Unit::handle_out_of_range(std::shared_ptr<AttackableGameObject> opponent)
{
	// Move towards opponent if in attack mode.
	if (currentCommand == UNIT_ATTACK) {
		set_destination(opponent.get());
		do_move();
	}
}

void Unit::handle_defeat(std::shared_ptr<AttackableGameObject> opponent)
{
	// Tell Player you have died.
	//player->add_to_destroy(this);
	//player = nullptr;
	send_update_to_manager(make_update());
	this->set_command(Unit::UNIT_DIE);
	LOG_DEBUG("Unit " + std::to_string(this->getID()) + " set to UNIT_DIE.");
}

void Unit::handle_victory(std::shared_ptr<AttackableGameObject> opponent)
{
	// Go back to idle if unit was attacking
	currentCommand = (currentCommand == UNIT_ATTACK) ? UNIT_IDLE : currentCommand;
	
	// TODO: Gain experience (?)
	player->increase_player_score(opponent->getAttack().getDamage());
}

void Unit::handle_counter(std::shared_ptr<AttackableGameObject> opponent) {
	// Unit has been attacked, notify 
	send_update_to_manager(make_update());
}

void Unit::send_update_to_manager(std::shared_ptr<UnitUpdate> update) {
	if (this->manager != nullptr) {
		this->manager->register_update(update);
	}
}
