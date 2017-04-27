/**
@file game_object.h
@brief An abstract class that defines basic game object.
*/
#pragma once
#include "drawable.h"
#include "identifiable.h"
#include "lib.h"
#include "player.h"
#include "game_object_update.h"
#include <glm\vec3.hpp>

/**
An abstract class that defines base game object.
*/
class GameObject : public Drawable, public Identifiable {
protected:
	Player* player;
	glm::vec3 position;

public:

	GameObject(glm::vec3 pos);
	GameObject(glm::vec3 pos, Player* assigned_player);

	/* If we are specifying the ID, we should have all information */
	GameObject(UID id, glm::vec3 pos, Player* assigned_player);

	/* Some game objects dont have players, like planets (should they be another class? maybe) */
	GameObject(UID id, glm::vec3 pos);

	/**
	Returns the player who owns this game object.
	@return The owner of this game object.
	*/
	Player* get_player();

	/* *
	Assign player while no constructor is doing so.
	*/
	void set_player(Player*);

	/**
	Returns the current position of this game object.
	@return The current position of this game object.
	*/
	glm::vec3 get_position();

	/**
	Updates the position of this game object
	@param The new position of the game object
	*/
	void update_position(glm::vec3 new_position);
};
