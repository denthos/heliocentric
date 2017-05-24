#pragma once

#include "game_object.h"
#include "spherical_coordinate.h"
#include "resources.h"


class SlotUpdate;

class Planet;
class City;

class Slot : public GameObject {
public:
	friend class SlotUpdate;

	Slot(Planet* planet, SphericalCoordinate coord);
	Slot(UID id, Planet* planet, SphericalCoordinate coord);

	glm::vec3 get_position() const;
	SphericalCoordinate get_spherical_position() const;

	Player* get_player() const;

	void attachCity(City* city);
	void detachCity();
	bool hasCity() const;
	City* getCity() const;

	Planet* getPlanet() const;

	int getResourceCount(Resources::Type resource) const;
	const std::unordered_map<Resources::Type, int>& getResources() const;
	void changeResourceCount(Resources::Type resource, int new_amount);

protected:
	SphericalCoordinate coordinate;
	Planet* planet;
	City* city;

private:
	std::unordered_map<Resources::Type, int> resources;
};
