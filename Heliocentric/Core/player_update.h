#pragma once

#include "identifiable.h"
#include "player.h"

class PlayerUpdate {
public:
	UID id;
	char player_name[PLAYER_NAME_MAX_SIZE];

	/**
	Constructor for a player update.
	@param id UID of the planet to be updated.
	@param player_name Name of the player to be updated.
	*/
	PlayerUpdate(UID, const char*);
	void apply(Player* player);
};