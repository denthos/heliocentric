#pragma once

#include "identifiable.h"
#include "player.h"

class PlayerUpdate {
public:
	UID id;
	char player_name[PLAYER_NAME_MAX_SIZE];

	void apply(Player* player);
};