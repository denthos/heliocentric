#include "player_client_to_server_xfer.h"

void PlayerClientToServerTransfer::apply(Player* player) {
	player->set_name(this->name);
}