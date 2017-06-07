#pragma once

#include "identifiable.h"
#include "city.h"

class CityUpdate {
public:
	UID id;
	int health;
	int production;
	int research_points;
	/**
	Constructor for a city update.
	@param id UID of the city to be updated.
	@param int health New city health.
	@param int production New city production.
	@param int research_points New city research points.
	*/
	CityUpdate(UID, int, int, int);
	void apply(City* city);
};