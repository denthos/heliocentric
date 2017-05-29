#pragma once
#include "unit.h"

class HeavyUnit : public Unit {
public:
	HeavyUnit(glm::vec3 position, Player* owner, UnitManager* manager, UnitType* type);
	HeavyUnit(UID id,glm::vec3 position, Player* owner, UnitManager* manager, UnitType* type);
};