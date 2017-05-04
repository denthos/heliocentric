#pragma once

#include <string>

#include "player.h"
#include "lib.h"

class PlayerClientToServerTransfer {
public:
	char name[PLAYER_NAME_MAX_SIZE];

	/**
	Constructor for PlayerClientToServerTransfer, which contains information of
	the client that the server does not yet know.
	@param name Name of the player.
	*/
	PlayerClientToServerTransfer(std::string);
	void apply(Player* player);
};