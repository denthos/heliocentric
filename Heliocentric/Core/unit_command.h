#pragma once

#include "base_command.h"

/**
This class works as a medium for client-to-server communication by carrying
information about user input on the client side. This class contains all
possible commands that the user will be able to execute on a unit, which
includes attack, move, and sleep.
*/

class UnitCommand : public BaseCommand {
public:
	UID target; // Unit to be attacked
	float destination_x; // Destination that the unit is moving towards
	float destination_y;
	float destination_z;
};