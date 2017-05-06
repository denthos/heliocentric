#include <glm/glm.hpp>
#include "universe.h"
#include "slot.h"

Universe::Universe() {
	/* Generate standard planets for this universe */
	this->planets.insert(std::make_unique<Planet>(glm::vec3(0.0f, 0.0f, 0.0f), "Sun", 0.0f, 20.0f, SUN, std::unordered_map<UID, Slot*>()));
	this->planets.insert(std::move(createPlanet(glm::vec3(50.0f, 0.0f, 0.0f), "Mercury", -2.0f, 3.0f, MERCURY)));
	this->planets.insert(std::move(createPlanet(glm::vec3(120.0f, 0.0f, 0.0f), "Venus", 1.5f, 4.0f, VENUS)));
	this->planets.insert(std::move(createPlanet(glm::vec3(300.0f, 0.0f, 0.0f), "Earth", 1.0f, 5.0f, EARTH)));
	this->planets.insert(std::move(createPlanet(glm::vec3(350.0f, 0.0f, 0.0f), "Mars", 1.0f, 4.5f, MARS)));
	this->planets.insert(std::move(createPlanet(glm::vec3(500.0f, 0.0f, 0.0f), "Jupiter", -0.5f, 10.0f, JUPITER)));
}

/**
A wrapper around the planet constructor that also creates two slots per planet. 
*/
std::unique_ptr<Planet> Universe::createPlanet(glm::vec3 position, std::string name, float speed, float radius, PlanetType type) {
	std::unordered_map<UID, Slot*> slots;

	Slot* left_slot = new Slot(radius * glm::vec3(0.9f, 0.0f, 0.0f));
	Slot* right_slot = new Slot(radius * glm::vec3(-0.9f, 0.0f, 0.0f));
	slots.insert(std::make_pair(left_slot->getID(), left_slot));
	slots.insert(std::make_pair(right_slot->getID(), right_slot));

	auto planet = std::make_unique<Planet>(position, name, speed, radius, type, slots);
	left_slot->planet = planet.get();
	right_slot->planet = planet.get();

	return planet;
}

void Universe::doLogic() {
	this->planet_updates.clear();
	for (auto& planet : this->planets) {
		planet->doLogic();
		this->planet_updates.push_back(planet->makeUpdate());
	}
}

std::set<std::unique_ptr<Planet>>& Universe::get_planets() {
	return this->planets;
}

std::vector<std::shared_ptr<PlanetUpdate>>& Universe::get_updates() {
	return this->planet_updates;
}
