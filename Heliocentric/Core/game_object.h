/**
@file game_object.h
@brief An abstract class that defines basic game object.
*/
#pragma once
#include "drawable.h"
#include "identifiable.h"
#include "lib.h"
#include "player.h"
#include <glm\vec3.hpp>

/**
An abstract class that defines base game object.
*/
class GameObject : public Drawable, public Identifiable {
protected:
	Player* player;
	glm::vec3 position;

public:
	GameObject();
	GameObject(UID id);
	GameObject(UID id, glm::vec3 pos);
	GameObject(UID id, glm::vec3 pos, Player* assigned_player);

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
	Set the position of an object.
	*/
	void set_position(glm::vec3 pos);
};