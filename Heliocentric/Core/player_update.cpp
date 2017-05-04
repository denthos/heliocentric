#include "player_update.h"
#include "lib.h"

PlayerUpdate::PlayerUpdate(UID id, const char* player_name) : id(id) {
	strcpy_s(this->player_name, sizeof(this->player_name), player_name);
}

void PlayerUpdate::apply(Player* player) {}