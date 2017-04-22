#pragma once

#include "game_object.h"
#include "player.h"
#include <vector>
#include <iostream>

struct SlotUpdate;

class Slot : public GameObject {
public:
	friend SlotUpdate;
	Slot(UID id, glm::vec3 pos);
	bool is_occupied();

private:
	//std::list<Resource> resources;
};