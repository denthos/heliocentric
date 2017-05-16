#include "unit_sample.h"
#include "instant_laser_attack.h"

UnitSample::UnitSample(glm::vec3 position, Player* owner) :
	Unit(position, owner, new InstantLaserAttack(), 100, 200) {

	this->movement_speed= 300;

	this->currentCommand = UNIT_IDLE;
}