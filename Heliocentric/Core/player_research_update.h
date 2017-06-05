#pragma once

#include "identifiable.h"
#include "player.h"

/**
Used to progress player through tech tree on client side.
*/

class PlayerResearchUpdate {
public:
	UID id; // player id
	int tech_id;
	float research_points;

	PlayerResearchUpdate(UID id, int tech_id, float research_points);
	void apply(Player* player);
};