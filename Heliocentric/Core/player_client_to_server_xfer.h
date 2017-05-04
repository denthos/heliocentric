#pragma once

#include <string>

#include "player.h"

class PlayerClientToServerTransfer {
public:
	char name[PLAYER_NAME_MAX_SIZE];

	void apply(Player* player);
};