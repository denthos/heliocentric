#pragma once

#include "game_object.h"
#include "slot.h"
#include <vector>
#include <string>
#include <iostream>

class PlanetUpdate;

class Planet : public GameObject {
public:
	friend PlanetUpdate;
	Planet();
	Planet(std::string planet_name, std::unordered_map<UID, Slot*>);
	std::unordered_map<UID, Slot*> get_slots();
	Slot* get_slot(UID);
	bool check_occupancy();

private:
	std::string name;       // planet name
	std::unordered_map<UID, Slot*> slots;  // list of slots available
	bool upgraded;          // true if all slots owned by one player
	//private std::list<Resource> resources;
};
