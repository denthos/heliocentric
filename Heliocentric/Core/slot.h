#pragma once

#include "game_object.h"
#include "player.h"
#include <vector>
#include <iostream>

struct SlotUpdate;
//class Planet;

class Slot : public GameObject {
public:
	friend SlotUpdate;
	Slot(UID, glm::vec3);
	bool is_occupied();

private:
	//Planet* planet;
	//std::list<Resource> resources;
};