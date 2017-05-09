#include "player_command.h"

PlayerCommand::PlayerCommand(float x, float y, float z) : BaseCommand::BaseCommand(0, CMD_CREATE),
	create_location_x(x), create_location_y(y), create_location_z(z) {

}

PlayerCommand::PlayerCommand(UID recipient, Resources::ResourceType selling, int sell_amount) :
	BaseCommand::BaseCommand(0, CMD_TRADE), trade_recipient(recipient), trade_selling(selling), trade_sell_amount(sell_amount) {

}