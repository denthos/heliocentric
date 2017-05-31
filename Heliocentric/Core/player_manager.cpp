#include "player_manager.h"


PlayerManager::PlayerManager() {}

void PlayerManager::doLogic() {
	player_score_updates.clear();
}

void PlayerManager::register_update(std::shared_ptr<PlayerScoreUpdate>& update) {
	player_score_updates.insert(update);
}

const std::unordered_set<std::shared_ptr<PlayerScoreUpdate>>& PlayerManager::getPlayerScoreUpdates() const {
	return this->player_score_updates;
}