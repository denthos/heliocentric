#include "new_player_info_update.h"

NewPlayerInfoUpdate::NewPlayerInfoUpdate(UID id, std::string name) : player_id(id) {
	snprintf(this->name, PLAYER_NAME_MAX_SIZE, "%s", name.c_str());
}