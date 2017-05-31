#include "heavy_unit.h"
#include "instant_laser_attack.h"

HeavyUnit::HeavyUnit(glm::vec3 position, Player* owner, UnitManager* manager, UnitType* type) :
	Unit(position, owner, new InstantLaserAttack(), manager, 100, 400, 1.0f, type) {}


HeavyUnit::HeavyUnit(UID id, glm::vec3 position, Player* owner, UnitManager* manager, UnitType* type) :
	Unit(id, position, owner, new InstantLaserAttack(), manager, 100, 400, 1.0f, type) {}