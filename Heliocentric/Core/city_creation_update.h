#pragma once
#include "identifiable.h"

class CityCreationUpdate {
public:
	CityCreationUpdate(UID, UID, UID);

	UID player_id;
	UID slot_id;
	UID city_id;

	// TODO: Add sending of city information ^^
};