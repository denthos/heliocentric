#include "basic_unit.h"
#include "instant_laser_attack.h"

BasicUnit::BasicUnit(glm::vec3 position, Player* owner, UnitManager* manager, UnitType* type) :
	Unit(position, owner, new InstantLaserAttack(), manager, 100, 200, 2.0f, type) {}

BasicUnit::BasicUnit(UID id, glm::vec3 position, Player* owner, UnitManager* manager, UnitType* type) :
	Unit(id, position, owner, new InstantLaserAttack(), manager, 100, 200, 2.0f, type) {}