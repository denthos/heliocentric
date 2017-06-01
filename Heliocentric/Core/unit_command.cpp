#include "unit_command.h"

UnitCommand::UnitCommand(UID initiator, UID target) : BaseCommand::BaseCommand(initiator, CMD_ATTACK), target(target) {

}

UnitCommand::UnitCommand(UID initiator, float x, float y, float z, float fx, float fy, float fz) : BaseCommand::BaseCommand(initiator, CMD_MOVE),
						destination_x(x), destination_y(y), destination_z(z), force_x(fx), force_y(fy), force_z(fz) {

}