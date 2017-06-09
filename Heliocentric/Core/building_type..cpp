#include "building_type.h"
#include "fission_plant.h"
#include "fusion_plant.h"
#include "research_facility.h"

BuildingType::BuildingType(BuildType buildType, int productionCost) : Buildable(Buildable::BuildType::BUILDING, productionCost) {

}

BuildingType* BuildingType::getByIdentifier(TypeIdentifier identifier) {
	return buildingTypeMap[identifier];
}

std::unordered_map<BuildingType::TypeIdentifier, BuildingType*> BuildingType::buildingTypeMap = {
	{
		BuildingType::TypeIdentifier::FISSION_PLANT,
		new BuildingTypeImpl<FissionPlant>(TypeIdentifier::FISSION_PLANT, 500, "Fission Power Plant", FISSION_PLANT_PRODUCTION, 0, 
		ResourceCollection {{Resources::URANIUM, 100}, {Resources::GOLD, 50}})
	},

	{
		BuildingType::TypeIdentifier::FUSION_PLANT,
		new BuildingTypeImpl<FusionPlant>(TypeIdentifier::FUSION_PLANT, 1000, "Fusion Power Plant", FUSION_PLANT_PRODUCTION, 0,
		ResourceCollection {{Resources::URANIUM, 200}, {Resources::NANOMATERIAL, 100}, {Resources::GOLD, 75}})
	},

};