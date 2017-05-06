#pragma once

#include "game_object.h"
#include "player.h"
#include "city.h"
#include <vector>
#include <iostream>

class SlotUpdate;
class Planet;

/**
Note that a slot's position is translation vector from the 
planet's center
*/
class Slot : public GameObject {
public:
	friend SlotUpdate;

	/* The planet that the slot is hosted on */
	Planet* planet;

	/* The city that is in this slot (NULL if not city yet) */
	City* city;

	/* A function to create a city on this slot (delegation to the city's construtor) */
	virtual void foundCity(Player* owner, int att, int def, int range, int heal, int pr, int pop);

	Slot(glm::vec3 pos);
	Slot(UID id,glm::vec3 pos);

	bool is_occupied();


private:
	//std::list<Resource> resources;
};