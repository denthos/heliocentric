#pragma once
#include "player.h"

class NewPlayerInfoUpdate {
public:
	UID player_id;
	char name[PLAYER_NAME_MAX_SIZE];
	PlayerColor::Color color;

	NewPlayerInfoUpdate(UID id, std::string name, PlayerColor::Color color);
	void apply(Player*);
};