#pragma once

#include "identifiable.h"
#include "city.h"

class CityUpdate {
public:
	UID id;
	void apply(City* city);
};