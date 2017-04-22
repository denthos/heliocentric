#include "attackable_upgradable.h"

AttackableUpgradable::AttackableUpgradable(int att_delt, int def_delt, int range_delt) : Upgradable<Attackable>(), attack_delta(att_delt), defense_delta(def_delt), range_delta(range_delt) {};
AttackableUpgradable::AttackableUpgradable(int att_delt, int def_delt, int range_delt, int max) : Upgradable<Attackable>(max), attack_delta(att_delt), defense_delta(def_delt), range_delta(range_delt) {};

void AttackableUpgradable::apply_upgrade(Attackable object) {
	if (check_max_tier(object))
		return;

	object.set_combat_attack(object.get_combat_attack() + attack_delta);
	object.set_combat_defense(object.get_combat_defense() + defense_delta);
	object.set_combat_range(object.get_combat_range() + range_delta);
}

bool AttackableUpgradable::check_max_tier(Attackable object) {
	return object.get_tier() >= MAX_TIER;
}