#include "base_command.h"

BaseCommand::BaseCommand(UID initiator, CommandType command_type) : initiator(initiator), command_type(command_type) {}