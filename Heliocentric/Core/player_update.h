#pragma once

#include <unordered_map>

#include "identifiable.h"
#include "player.h"
#include "resources.h"
#include "logging.h"

class PlayerUpdate {
public:
	UID id;
	char player_name[PLAYER_NAME_MAX_SIZE];

	int gold_change; // just support gold for now

	/**
	Constructor for a player update.
	@param id UID of the planet to be updated.
	@param player_name Name of the player to be updated.
	*/
	PlayerUpdate(UID, const char*);
	PlayerUpdate(UID, Resources::ResourceType, int);
	void apply(Player* player);
};