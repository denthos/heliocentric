#pragma once
#include "upgradable.h"
#include "attackable.h"

class AttackableUpgradable : Upgradable<Attackable> {
public:
	AttackableUpgradable(int, int, int);
	AttackableUpgradable(int, int, int, int);
	void apply_upgrade(Attackable*);
	bool is_max_tier(Attackable*);

protected:
	int attack_delta;
	int defense_delta;
	int range_delta;
	// int healthDelta;         // if we ever need to change health
};
