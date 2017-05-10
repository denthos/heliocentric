#include "slot.h"
#include "planet.h"

Slot::Slot(Planet* planet, SphericalCoordinate coord) : planet(planet), coordinate(coord), city(NULL) {}

Slot::Slot(UID id, Planet* planet, SphericalCoordinate coord) : Identifiable(id), planet(planet), coordinate(coord), city(NULL) {}

bool Slot::hasCity() const {
	return (this->city != NULL);
}

City* Slot::getCity() const {
	return this->city;
}

void Slot::attachCity(City* city) {
	this->city = city;
}

glm::vec3 Slot::get_absolute_position() const {
	return this->coordinate.toCartesian(this->planet->get_position(), this->planet->get_radius());
}

SphericalCoordinate Slot::get_spherical_position() const {
	return this->coordinate;
}

Planet* Slot::getPlanet() const {
	return this->planet;
}
