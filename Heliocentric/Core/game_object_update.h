#pragma once

#include "identifiable.h"
#include "game_object.h"

class GameObjectUpdate {
public:
	UID id;
	float x, y, z;

	/**
	Constructor for a GameObjectUpdate.
	@param id UID of the game object to be updated.
	@param x x-axis of the object's new position.
	@param y y-axis of the object's new position.
	@param z z-axis of the object's new position.
	*/
	GameObjectUpdate(UID, float, float, float);
	void apply(GameObject* game_object);
};