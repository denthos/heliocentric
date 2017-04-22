#include "city_upgradable.h"

CityUpgradable::CityUpgradable(int att_delta, int def_delta, int range_delta, int prod_delta, int pop_delta) : AttackableUpgradable(att_delta, def_delta, range_delta), production_delta(prod_delta), population_delta(pop_delta) {};
CityUpgradable::CityUpgradable(int att_delta, int def_delta, int range_delta, int prod_delta, int pop_delta, int max) : AttackableUpgradable(att_delta, def_delta, range_delta, max), production_delta(prod_delta), population_delta(pop_delta) {};

void CityUpgradable::apply_upgrade(City object) {
	if (check_max_tier(object))
		return;

	AttackableUpgradable::apply_upgrade(object);
	object.set_production(object.get_production() + production_delta);
	object.set_population(object.get_population() + population_delta);
}