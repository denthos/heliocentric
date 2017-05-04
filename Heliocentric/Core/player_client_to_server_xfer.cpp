#include "player_client_to_server_xfer.h"

PlayerClientToServerTransfer::PlayerClientToServerTransfer(std::string name) {
	Lib::assertTrue(name.size() < PLAYER_NAME_MAX_SIZE, "Player name length invalid!");
	snprintf(this->name, PLAYER_NAME_MAX_SIZE, "%s", name.c_str());
}

void PlayerClientToServerTransfer::apply(Player* player) {
	player->set_name(this->name);
}