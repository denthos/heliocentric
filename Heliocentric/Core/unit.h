#pragma once

#include "attackable.h"
#include "game_object.h"

struct UnitUpdate;

class Unit : public GameObject, public Attackable {
public:
	friend UnitUpdate;
	void update();
	glm::vec3 get_destination();
	glm::vec3 set_destination(glm::vec3 destination);
	int get_movement_speed_max();
	int set_movmennt_speed_max(int movementSpeedMax);

protected:
	Unit* do_attack(Unit* target);
	glm::vec3 do_move();

	enum CommandType { attack, idle, move };

	int movementSpeedMax; // maximum speed that this unit can achieve when powered by its own engine
	int movementSpeedCurrent; // can be used if implementing gravity simulation
	CommandType currentCommand;
	glm::vec3 destination;
};