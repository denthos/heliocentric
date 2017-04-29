#pragma once
#include "identifiable.h"

class UnitCreateCommand {
public:
	UID player_id;

	float x;
	float y;
	float z;

	/* TODO: Include things like */
	// UNIT_TYPE unit_type;

	// Later we would probably want to have things like health, armor, attack
	// here so we can change them based on the player's technologies
};