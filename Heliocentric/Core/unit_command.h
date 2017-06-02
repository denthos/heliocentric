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
	bool high_pri;

	/**
	Constructor for a CMD_ATTACK type command.
	Currently only this command takes two UID's, so no command type needs to be passed in as a parameter.
	@param initiator UID of the object that will initiate an attack.
	@param target UID of the object that the initiator is going to attack.
	*/
	UnitCommand(UID, UID);

	/**
	Constructor for a CMD_MOVE type command.
	@param initiator UID of the object that should move to the destination.
	@param destination_x x-axis of the destination that the initiator will move to.
	@param destination_y y-axis of the destination that the initiator will move to.
	@param destination_z z-axis of the destination that the initiator will move to.
	*/
	UnitCommand(UID, float, float, float, bool);
};