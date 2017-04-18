/**
@file game_object.h
@brief An abstract class that defines basic game object.
*/
#pragma once
#include "lib.h"
#include "player.h"
#include <glm\vec3.hpp>

/**
An abstract class that defines base game object.
*/
class GameObject {
protected:
	Player* player;
	glm::vec3 position;
public:
	/**
	Returns the player who owns this game object.
	@return The owner of this game object.
	*/
	virtual Player* get_player();

	/**
	Returns the current position of this game object.
	@return The current position of this game object.
	*/
	virtual glm::vec3 get_position();
};