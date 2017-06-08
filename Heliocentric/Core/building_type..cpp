#include "building_type.h"
#include "fission_plant.h"
#include "fusion_plant.h"
#include "research_facility.h"

BuildingType* BuildingType::getByIdentifier(TypeIdentifier identifier) {
	return buildingTypeMap[identifier];
}

std::unordered_map<BuildingType::TypeIdentifier, BuildingType*> BuildingType::buildingTypeMap = {
	{
		BuildingType::TypeIdentifier::FISSION_PLANT,
		new BuildingTypeImpl<FissionPlant>(TypeIdentifier::FISSION_PLANT, 100, "Fission Power Plant")
	},

	{
		BuildingType::TypeIdentifier::FUSION_PLANT,
		new BuildingTypeImpl<FusionPlant>(TypeIdentifier::FUSION_PLANT, 200, "Fusion Power Plant")
	}
};