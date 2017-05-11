#pragma once

#include "identifiable.h"
#include "spherical_coordinate.h"
#include "resources.h"


class SlotUpdate;

class Planet;
class City;


class Slot : public Identifiable {
public:
	friend SlotUpdate;

	Slot(Planet* planet, SphericalCoordinate coord);
	Slot(UID id, Planet* planet, SphericalCoordinate coord);

	glm::vec3 get_absolute_position() const;
	SphericalCoordinate get_spherical_position() const;

	void attachCity(City* city);
	bool hasCity() const;
	City* getCity() const;

	Planet* getPlanet() const;


protected:
	Resources::ResourceType resource_type;
	float resource_deposit;
	SphericalCoordinate coordinate;
	Planet* planet;
	City* city;
};
