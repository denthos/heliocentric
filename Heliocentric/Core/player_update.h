#pragma once

#include "identifiable.h"
#include "player.h"

class PlayerUpdate {
public:
	UID id;

	void apply(Player* player);
};