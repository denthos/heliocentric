#pragma once

#pragma once
#include <set>
#include <memory>
#include <unordered_set>
#include <vector>
#include "player.h"
#include "player_score_update.h"
#include "player_research_update.h"

class PlayerManager {
private:
	std::vector<std::shared_ptr<Player>> players;
	std::unordered_set<std::shared_ptr<PlayerScoreUpdate>> player_score_updates;
	std::unordered_set<std::shared_ptr<PlayerResearchUpdate>> player_research_updates;

public:
	PlayerManager();

	void doLogic();

	void register_player(std::shared_ptr<Player> player);
	void register_update(std::shared_ptr<PlayerScoreUpdate> update);
	void register_update(std::shared_ptr<PlayerResearchUpdate> update);

	const std::unordered_set<std::shared_ptr<PlayerScoreUpdate>>& getPlayerScoreUpdates() const;
	const std::unordered_set<std::shared_ptr<PlayerResearchUpdate>>& getPlayerResearchUpdates() const;
};
