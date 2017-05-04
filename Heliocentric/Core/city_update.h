#pragma once

#include "identifiable.h"
#include "city.h"

class CityUpdate {
public:
	UID id;

	/**
	Constructor for a city update.
	@param id UID of the city to be updated.
	*/
	CityUpdate(UID);
	void apply(City* city);
};