#include <glm/gtc/matrix_transform.hpp>

#include "planet.h"
#include "planet_update.h"


Planet::Planet(glm::vec3 position, std::string planet_name, float orbit_speed, float radius, PlanetType type, std::unordered_map<UID, Slot*> map, std::unordered_map<Resources::Type, int> resources) :
	GameObject(position), name(planet_name), slots(map), orbit_speed(orbit_speed), radius(radius), type(type), resources(resources) {
	initialize();
}

Planet::Planet(UID id, glm::vec3 position, std::string planet_name, float orbit_speed, float radius, PlanetType type, std::unordered_map<UID, Slot*> map, std::unordered_map<Resources::Type, int> resources) :
	GameObject(id, position), name(planet_name), slots(map), orbit_speed(orbit_speed), radius(radius), type(type), resources(resources) {
	initialize();
}

void Planet::initialize() {
	this->update = std::make_shared<PlanetUpdate>(getID(), position.x, position.y, position.z);
}

void Planet::distributeResourcesAmongstSlots() {
	/* Randomly distribute resources amongst slots. */
	for (auto slot : slots) {

		for (auto resource_pair : resources) {
			int resource_amount = rand() % resource_pair.second;

			if (resource_amount > 0) {
				resources[resource_pair.first] -= resource_amount;
				slot.second->changeResourceCount(resource_pair.first, resource_amount);
			}
		}

	}
}

std::string Planet::getName() const {
	return name;
}


std::unordered_map<UID, Slot*>& Planet::get_slots() {
	return slots;
}

const std::unordered_map<UID, Slot*>& Planet::get_slots_const() const {
	return slots;
}

Slot* Planet::get_slot(UID id) {
	return slots[id];
}

bool Planet::check_occupancy() const {
	if (slots.size() == 0) {
		return false;
	}

	for (auto pair : slots) {
		if (pair.second->hasCity())
			return false;
	}

	return true;
}

void Planet::doLogic() {
	glm::vec3 new_position = (
		glm::rotate(glm::mat4(1.0f), (this->orbit_speed / 180.0f) * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) *
		glm::vec4(this->position, 1.0f)
	);

	this->update_position(new_position);
}

std::shared_ptr<PlanetUpdate> Planet::makeUpdate() {
	this->update->id = this->getID();
	this->update->x = this->position.x;
	this->update->y = this->position.y;
	this->update->z = this->position.z;

	return this->update;
}

float Planet::get_radius() const {
	return this->radius;
}

PlanetType Planet::get_type() const {
	return this->type;
}
