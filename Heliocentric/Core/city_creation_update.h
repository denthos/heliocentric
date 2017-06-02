#pragma once
#include "identifiable.h"
#include "city.h"

class CityCreationUpdate {
public:
	CityCreationUpdate(UID player_id, UID slot_id, UID city_id, std::string name,
		int defense, int health, int production, int population);

	UID player_id;
	UID slot_id;
	UID city_id;

	int defense;
	int health;
	int production;
	int population;


	char name[CITY_NAME_SIZE];

	// TODO: Add sending of city information ^^
};