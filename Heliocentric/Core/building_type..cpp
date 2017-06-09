#include "building_type.h"
#include "fission_plant.h"
#include "fusion_plant.h"
#include "research_facility.h"
#include "fortress.h"

BuildingType::BuildingType(BuildType buildType, int productionCost) : Buildable(Buildable::BuildType::BUILDING, productionCost) {

}

BuildingType* BuildingType::getByIdentifier(TypeIdentifier identifier) {
	return buildingTypeMap[identifier];
}

std::unordered_map<BuildingType::TypeIdentifier, BuildingType*> BuildingType::buildingTypeMap = {
	{
		BuildingType::TypeIdentifier::FISSION_PLANT,
		new BuildingTypeImpl<FissionPlant>(TypeIdentifier::FISSION_PLANT, FISSION_PLANT_COST, "Fission Power Plant", 0, FISSION_PLANT_PRODUCTION, 0, 
		ResourceCollection {{Resources::URANIUM, 100}, {Resources::GOLD, 50}})
	},

	{
		BuildingType::TypeIdentifier::FUSION_PLANT,
		new BuildingTypeImpl<FusionPlant>(TypeIdentifier::FUSION_PLANT, FUSION_PLANT_COST, "Fusion Power Plant", 0, FUSION_PLANT_PRODUCTION, 0,
		ResourceCollection {{Resources::URANIUM, 200}, {Resources::NANOMATERIAL, 100}, {Resources::GOLD, 75}})
	},
	
	{
		BuildingType::TypeIdentifier::RESEARCH_FACILITY,
		new BuildingTypeImpl<ResearchFacility>(TypeIdentifier::RESEARCH_FACILITY, RESEARCH_FACILITY_COST, "Research Facility", 0, RESEARCH_FACILITY_SCIENCE,
		ResourceCollection {{Resources::GOLD, 50}, {Resources::TITANIUM, 100}})
	},

	{
		BuildingType::TypeIdentifier::FORTRESS,
		new BuildingTypeImpl<Fortress>(TypeIdentifier::FORTRESS, FORTRESS_COST, "Fortress", FORTRESS_ARMOR, 0, 0,
		ResourceCollection {{Resources::TITANIUM, 50}})
	}

};
