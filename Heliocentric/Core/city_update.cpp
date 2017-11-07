#include "city_update.h"
#include "spawner_update.h"

CityUpdate::CityUpdate(UID id, int health, int production, int research_points) :
	id(id), health(health), production(production), research_points(research_points) {}

void CityUpdate::apply(City* city) {
	city->set_health(this->health);
	city->set_production(this->production);
	city->set_research_points(this->research_points);
}