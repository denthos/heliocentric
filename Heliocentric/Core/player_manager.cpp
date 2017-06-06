#include "player_manager.h"


PlayerManager::PlayerManager() {}

void PlayerManager::doLogic() {
	player_score_updates.clear();
	player_research_updates.clear();

	if (players.size() > 0) {
		for (auto it : players) {
			it->research();
		}
	}
}

void PlayerManager::register_player(std::shared_ptr<Player> player) {
	LOG_DEBUG("Registering player ", player->getID(), " to player manager");
	players.push_back(player);
}

void PlayerManager::register_update(std::shared_ptr<PlayerScoreUpdate> update) {
	player_score_updates.insert(update);
}

void PlayerManager::register_update(std::shared_ptr<PlayerResearchUpdate> update) {
	player_research_updates.insert(update);
}

const std::unordered_set<std::shared_ptr<PlayerScoreUpdate>>& PlayerManager::getPlayerScoreUpdates() const {
	return this->player_score_updates;
}

const std::unordered_set<std::shared_ptr<PlayerResearchUpdate>>& PlayerManager::getPlayerResearchUpdates() const {
	return this->player_research_updates;
}
