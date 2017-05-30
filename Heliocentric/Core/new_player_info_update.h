#pragma once
#include "player.h"

class NewPlayerInfoUpdate {
public:
	UID player_id;
	char name[PLAYER_NAME_MAX_SIZE];

	NewPlayerInfoUpdate(UID id, std::string name);
};