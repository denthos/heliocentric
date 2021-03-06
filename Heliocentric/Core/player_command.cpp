#include "player_command.h"

PlayerCommand::PlayerCommand(float x, float y, float z, UnitType::TypeIdentifier create_type, UID cityID) : BaseCommand::BaseCommand(cityID, CMD_CREATE_UNIT),
	create_location_x(x), create_location_y(y), create_location_z(z), createUnitType(create_type) { }

PlayerCommand::PlayerCommand(BuildingType::TypeIdentifier create_type, UID cityID) : BaseCommand::BaseCommand(cityID, CMD_CREATE_BUILDING), createBuildingType(create_type) {

}

PlayerCommand::PlayerCommand(UID recipient, Resources::Type selling, int sell_amount) :
	BaseCommand::BaseCommand(0, CMD_TRADE), trade_recipient(recipient), trade_selling(selling), trade_sell_amount(sell_amount) {

}
