#pragma once
#include "base_command.h"
#include "identifiable.h"

class TradeCommand : public BaseCommand {
public:
	TradeCommand(UID, bool);
};