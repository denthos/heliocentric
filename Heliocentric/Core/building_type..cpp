#include "building_type.h"

BuildingType* BuildingType::getByIdentifier(TypeIdentifier identifier) {
	return buildingTypeMap[identifier];
}

