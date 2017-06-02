#pragma once

#pragma once
#include <set>
#include <memory>
#include <unordered_set>
#include "player.h"
#include "player_score_update.h"

class PlayerManager {
private:
	std::unordered_set<std::shared_ptr<PlayerScoreUpdate>> player_score_updates;

public:
	PlayerManager();

	void doLogic();

	void register_update(std::shared_ptr<PlayerScoreUpdate> update);

	const std::unordered_set<std::shared_ptr<PlayerScoreUpdate>>& getPlayerScoreUpdates() const;
};
