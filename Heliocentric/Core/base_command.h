#pragma once

#include "identifiable.h"

/**
This class works as a medium for client-to-server communication by carrying
information about user input on the client side. This class contains general
information about a command, which includes the ID of the player that sent the
command, what the command is, and on which object (if any) did the player send
the command.
*/
class BaseCommand {
public:
	/* This enum should include all possible commands that a player can send. */
	enum CommandType { CMD_ATTACK, CMD_BUILD, CMD_CREATE_BUILDING, CMD_CREATE_UNIT,
	                   CMD_DELETE, CMD_MOVE, CMD_PURCHASE, CMD_SETTLE_CITY, CMD_SLEEP,
	                   CMD_TRADE_ACCEPT, CMD_TRADE_DECLINE, CMD_TRADE
	};

	UID initiator; // The object that will execute this command.
				   // uh... is there a better name for it?
	CommandType command_type; // Specifies what the command is.

	/**
	Constructor for the base command.
	@param initiator UID of the object that the player sent this command to.
	@param command_type Specifies the command type.
	*/
	BaseCommand(UID, CommandType);
};