#include "player_update.h"

PlayerUpdate::PlayerUpdate(UID id, Resources::Type resource_type, int amount) : id(id), resource_type(resource_type), new_resource_value(amount) {}

void PlayerUpdate::apply(Player* player) {
	player->set_resource_amount(resource_type, new_resource_value);
}