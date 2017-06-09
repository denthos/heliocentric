#include "building.h"
#include "building_type.h"

Building::Building(BuildingType* type) : production(type->getProduction()), research_points(type->getResearchPoints()) {

}