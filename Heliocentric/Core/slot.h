#pragma once

#include "game_object.h"
#include "player.h"
#include <vector>
#include <iostream>

class Slot : public GameObject {
public:
	Slot(int ID, bool occupied, glm::vec3 pos);
	int get_slot_ID();
	bool is_occupied();
	void assign_player(Player* owner);
	
private:
	int ID;
	//std::list<Resource> resources;
};
