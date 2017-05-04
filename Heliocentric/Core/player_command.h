#pragma once

#include "base_command.h"

/**
This class works as a medium for client-to-server communication by carrying
information about user input on the client side. This class contains all
possible commands that the user will be able to execute on an in-game player,
namely commands that will take effect on a global scale. These may include
declaring war on other players, selecting technology to research, and sending
trade deals to other players.

This class currently includes only a command to create a unit given a spawn
location. This command should eventually be moved to a CityCommand class,
because players should be building or potentially purchasing units only inside
cities. Current implementation should be temporary.
*/
class PlayerCommand : public BaseCommand {
public:
	float create_location_x; // The location where a unit will be created
	float create_location_y;
	float create_location_z;

	/* TODO: Include things like */
	// UNIT_TYPE unit_type;

	// Later we would probably want to have things like health, armor, attack
	// here so we can change them based on the player's technologies

	/**
	Constructor for a CMD_CREATE type command, which is a temporary command
	used to create a unit for testing.
	@param create_location_x x-axis of the location where the unit should be created.
	@param create_location_y y-axis of the location where the unit should be created.
	@param create_location_z z-axis of the location where the unit should be created.
	*/
	PlayerCommand(float, float, float);
};