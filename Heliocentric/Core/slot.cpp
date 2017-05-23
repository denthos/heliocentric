#include "slot.h"
#include "city.h"
#include "planet.h"

Slot::Slot(Planet* planet, SphericalCoordinate coord) :
	GameObject(coord.toCartesian(planet->get_position(), planet->get_radius())),
	planet(planet), coordinate(coord), city(NULL) {}

Slot::Slot(UID id, Planet* planet, SphericalCoordinate coord) :
	GameObject(id, coord.toCartesian(planet->get_position(), planet->get_radius())),
	planet(planet), coordinate(coord), city(NULL) {}

bool Slot::hasCity() const {
	return (this->city != NULL);
}

City* Slot::getCity() const {
	return this->city;
}

void Slot::attachCity(City* city) {
	this->city = city;
}

glm::vec3 Slot::get_position() const {
	return this->coordinate.toCartesian(this->planet->get_position(), this->planet->get_radius());
}

SphericalCoordinate Slot::get_spherical_position() const {
	return this->coordinate;
}

void Slot::changeResourceCount(Resources::Type resource, int new_amount) {
	this->resources[resource] = new_amount;
}

const std::unordered_map<Resources::Type, int>& Slot::getResources() const {
	return this->resources;
}

int Slot::getResourceCount(Resources::Type resource) const {
	auto& resource_count_pair = this->resources.find(resource);
	if (resource_count_pair == this->resources.end()) {
		return 0;
	}
	else {
		return resource_count_pair->second;
	}
}

Planet* Slot::getPlanet() const {
	return this->planet;
}

Player* Slot::get_player() const {
	if (!hasCity()) {
		return NULL;
	}

	return getCity()->get_player();
}
