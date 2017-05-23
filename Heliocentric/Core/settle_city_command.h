#pragma once
#include "base_command.h"
#include "city.h"

class SettleCityCommand : public BaseCommand {
public:
	SettleCityCommand(UID slotId, std::string name);

	char city_name[CITY_NAME_SIZE];
};