#include "player_score_update.h"

PlayerScoreUpdate::PlayerScoreUpdate(UID id, int new_score) : id(id), new_score(new_score) {}

void PlayerScoreUpdate::apply(Player* player) {
	player->increase_player_score(this->new_score - player->get_player_score());
}