#include "player_update.h"

PlayerUpdate::PlayerUpdate(UID id, const char* player_name) : id(id) {
	strcpy_s(this->player_name, sizeof(this->player_name), player_name);
	this->gold_change = 0; // With current scheme, unitialized gold change will cause client to update player's gold deposit whenever an update is received
}

PlayerUpdate::PlayerUpdate(UID id, Resources::Type resource_type, int amount) : id(id) {
	switch (resource_type) {
		case Resources::GOLD: {
			this->gold_change = amount;
			break;
		}
		default: {
			LOG_ERR("Not supported yet.");
		}
	}
	
}

void PlayerUpdate::apply(Player* player) {
	player->owned_resources[Resources::GOLD] += (float)gold_change; // not sure if this float casting works with glm
		                                                            // may need to change from int to float if it doesn't work
}