#include "building.h"

Building::Building(BuildingType* type) : production(type->getProduction()), research_points(type->getResearchPoints()) {

}