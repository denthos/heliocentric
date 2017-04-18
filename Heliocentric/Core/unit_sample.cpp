#include "unit_sample.h"

UnitSample::UnitSample(Player* player, glm::vec3 position) {
	this->combatAttack = 100;
	this->combatDefense = 100;
	this->combatRange = 200;
	this->health = 100;
	this->movementSpeedMax = 300;

	this->currentCommand = idle;
	this->player = player;
	this->position = position;
}