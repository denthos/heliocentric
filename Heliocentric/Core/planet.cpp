#include "planet.h"

Planet::Planet(std::string planet_name, std::vector<Slot*>) : name(planet_name), slots(std::vector<Slot*>()) {}

bool Planet::check_occupancy() {
	if (slots.size() == 0) {
		return false;
	}

	for (std::vector<Slot*>::iterator it = slots.begin(); it != slots.end(); ++it) {
		if (!(*it)->is_occupied())
			return false;
	}

	return true;
}

void Planet::print() {
	std::cout << "Planet " << name << " has " << slots.size() << " slots" << std::endl;
}