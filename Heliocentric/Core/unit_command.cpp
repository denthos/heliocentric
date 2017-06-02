#include "unit_command.h"

UnitCommand::UnitCommand(UID initiator, UID target) : BaseCommand::BaseCommand(initiator, CMD_ATTACK), target(target) {

}

UnitCommand::UnitCommand(UID initiator, float x, float y, float z, bool high_pri) : BaseCommand::BaseCommand(initiator, CMD_MOVE),
						destination_x(x), destination_y(y), destination_z(z), high_pri(high_pri) {

}