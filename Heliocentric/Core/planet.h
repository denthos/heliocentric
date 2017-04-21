#pragma once

#include "game_object.h"
#include "slot.h"
#include "player.h"
#include <vector>
#include <string>
#include <iostream>

class PlanetUpdate;

class Planet : public GameObject {
public:
	friend PlanetUpdate;
	Planet(std::string planet_name, std::vector<Slot*>);
	bool check_occupancy();
	void print();

private:
	std::string name;       // planet name
	std::vector<Slot*> slots;  // list of slots available
	bool upgraded;          // true if all slots owned by one player
	//private std::list<Resource> resources;
};
