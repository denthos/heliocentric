#pragma once

#include "identifiable.h"
#include "game_object.h"

class GameObjectUpdate {
public:
	UID id;
	float x, y, z;

	void apply(GameObject* game_object);
};