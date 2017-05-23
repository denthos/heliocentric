#include "city_creation_update.h"

CityCreationUpdate::CityCreationUpdate(UID player_id, UID slot_id, UID city_id, std::string name) :
	player_id(player_id), slot_id(slot_id), city_id(city_id) 
{

	snprintf(this->name, CITY_NAME_SIZE, "%s", name.c_str());

}