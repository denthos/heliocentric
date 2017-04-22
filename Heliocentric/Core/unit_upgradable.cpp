#include "unit_upgradable.h"

UnitUpgradable::UnitUpgradable(int att_delta, int def_delta, int range_delta, int movespeed_delta) : AttackableUpgradable(att_delta, def_delta, range_delta), movement_speed_max_delta(movespeed_delta) {};
UnitUpgradable::UnitUpgradable(int att_delta, int def_delta, int range_delta, int movespeed_delta, int max) : AttackableUpgradable(att_delta, def_delta, range_delta, max), movement_speed_max_delta(movespeed_delta) {};

void UnitUpgradable::apply_upgrade(Unit object) {
	if (check_max_tier(object))
		return;

	AttackableUpgradable::apply_upgrade(object);
	object.set_movement_speed_max(object.get_movement_speed_max());
}