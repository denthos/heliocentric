#pragma once
#include "attack.h"

class InstantLaserAttack : public Attack {
public:
	InstantLaserAttack() : Attack(500.0f, 0.0f, 100.0f, 2000.0f) {}
};