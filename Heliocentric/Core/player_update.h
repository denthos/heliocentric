#pragma once

#include <unordered_map>

#include "identifiable.h"
#include "player.h"
#include "resources.h"
#include "logging.h"

class PlayerUpdate {
public:
	UID id;

	Resources::Type resource_type;
	int new_resource_value; 

	/**
	Constructor for a player update.
	@param id UID of the player to be updated.
	@param type The type of resource to be updated
	@param amount The new amount of the resource
	*/
	PlayerUpdate(UID id, Resources::Type type, int amount);
	void apply(Player* player);
};