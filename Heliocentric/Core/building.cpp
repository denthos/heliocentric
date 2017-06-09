#include "building.h"
#include "building_type.h"

Building::Building(BuildingType* type) : armor(type->getArmor()), production(type->getProduction()), research_points(type->getResearchPoints()) {

}