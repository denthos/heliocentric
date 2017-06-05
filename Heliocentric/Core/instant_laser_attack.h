#pragma once
#include "attack.h"

class InstantLaserAttack : public Attack {
public:
	InstantLaserAttack() : Attack(100.0f, 1000.0f, 100.0f, 2000.0f) {}
};