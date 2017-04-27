#include "universe.h"
#include "slot.h"

Universe::Universe() {
	/* No concept of slots yet -- so let's just share slots amongst planets*/
	std::unordered_map<UID, Slot*> slots;

	/* Generate standard planets for this universe */
	this->planets.insert(std::make_unique<Planet>(glm::vec3(0.0f, 0.0f, 0.0f), "Sun", 0.0f, 20.0f, slots));
	this->planets.insert(std::make_unique<Planet>(glm::vec3(50.0f, 0.0f, 0.0f), "Mercury", -2.0f, 3.0f, slots));
	this->planets.insert(std::make_unique<Planet>(glm::vec3(120.0f, 0.0f, 0.0f), "Venus", 1.5f, 4.0f, slots));
	this->planets.insert(std::make_unique<Planet>(glm::vec3(300.0f, 0.0f, 0.0f), "Earth", 1.0f, 5.0f, slots));
	this->planets.insert(std::make_unique<Planet>(glm::vec3(500.0f, 0.0f, 0.0f), "Jupiter", -0.5f, 10.0f, slots));
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
