#pragma once

#include <unordered_map>

#include "identifiable.h"
#include "player.h"
#include "resources.h"
#include "logging.h"

class PlayerScoreUpdate {
public:
	UID id;

	int new_score;

	/**
	Constructor for a player update.
	@param id UID of the player to be updated.
	@param score The player's new score.
	*/
	PlayerScoreUpdate(UID id, int score);
	void apply(Player* player);
};