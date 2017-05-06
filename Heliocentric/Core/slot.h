#pragma once

#include "game_object.h"
#include "player.h"
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

	Planet* planet;

	Slot(glm::vec3 pos);
	Slot(UID id,glm::vec3 pos);

	bool is_occupied();

private:
	//std::list<Resource> resources;
};