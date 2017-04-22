#pragma once
#include "attackable_upgradable.h"
#include "unit.h"

class UnitUpgradable : Upgradable<Unit>, AttackableUpgradable {
public:
	UnitUpgradable(int, int, int, int);
	UnitUpgradable(int, int, int, int, int);
	void apply_upgrade(Unit);

private:
	int movement_speed_max_delta;
};