#include "unit_sample.h"

UnitSample::UnitSample(glm::vec3 position, Player* owner) :
	Unit(position, owner, 100, 100, 200, 100) {

	this->movement_speed= 300;

	this->currentCommand = idle;
}