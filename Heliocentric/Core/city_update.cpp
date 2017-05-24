#include "city_update.h"

CityUpdate::CityUpdate(UID id, int health) : id(id), health(health) {
	
}

void CityUpdate::apply(City* city) {
	city->set_health(this->health);
}