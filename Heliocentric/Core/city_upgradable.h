#pragma once
#include "upgradable.h"
#include "attackable.h"

class AttackableUpgradable : Upgradable<Attackable> {
public:
	void apply_stats_upgrade();

protected:
	int attackDelta;
	int defenseDelta;
	int rangeDelta;
	int healthDelta;
};
