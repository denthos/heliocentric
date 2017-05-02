#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "planet.h"
#include "planet_update.h"

Planet::Planet(glm::vec3 position, std::string planet_name, float orbit_speed, float radius, std::unordered_map<UID, Slot*> map) :
	GameObject(position), name(planet_name), slots(map), orbit_speed(orbit_speed), radius(radius) {
	this->update = std::make_shared<PlanetUpdate>();
}

Planet::Planet(UID id, glm::vec3 position, std::string planet_name, float orbit_speed, float radius, std::unordered_map<UID, Slot*> map) :
	GameObject(id, position), name(planet_name), slots(map), orbit_speed(orbit_speed), radius(radius) {
	this->update = std::make_shared<PlanetUpdate>();
}


std::unordered_map<UID, Slot*> Planet::get_slots() const {
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
		if (pair.second->is_occupied())
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
