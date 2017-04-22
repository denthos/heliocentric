#pragma once
#include "attackable_upgradable.h"
#include "city.h"

class CityUpgradable : Upgradable<City>, AttackableUpgradable {
public:
	CityUpgradable(int, int, int, int, int);
	CityUpgradable(int, int, int, int, int, int);

	void apply_upgrade(City);

private:
	int production_delta;
	int population_delta;
};