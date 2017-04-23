#include "planet.h"

Planet::Planet() : GameObject() {};
Planet::Planet(std::string planet_name, std::unordered_map<UID, Slot*> map) : name(planet_name), slots(map) {}

std::unordered_map<UID, Slot*> Planet::get_slots() {
	return slots;
}

Slot* Planet::get_slot(UID id) {
	return slots[id];
}

bool Planet::check_occupancy() {
	if (slots.size() == 0) {
		return false;
	}

	for (auto pair : slots) {
		if (pair.second->is_occupied())
			return false;
	}

	return true;
}