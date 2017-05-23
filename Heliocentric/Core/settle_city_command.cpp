#include "settle_city_command.h"

SettleCityCommand::SettleCityCommand(UID slotID, std::string name) : BaseCommand(slotID, CMD_SETTLE_CITY)
{

	snprintf(this->city_name, CITY_NAME_SIZE, "%s", name.c_str());

}