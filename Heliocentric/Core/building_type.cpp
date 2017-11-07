#include "building_type.h"
#include "fission_plant.h"
#include "fusion_plant.h"
#include "research_facility.h"
#include "fortress.h"
#include "hadron_collider.h"

BuildingType::BuildingType(BuildType buildType, int productionCost) : Buildable(Buildable::BuildType::BUILDING, productionCost) {

}

BuildingType* BuildingType::getByIdentifier(TypeIdentifier identifier) {
	return buildingTypeMap[identifier];
}

std::unordered_map<BuildingType::TypeIdentifier, BuildingType*> BuildingType::buildingTypeMap = {
	{
		BuildingType::TypeIdentifier::FISSION_PLANT,
		new BuildingTypeImpl<FissionPlant>(TypeIdentifier::FISSION_PLANT, FISSION_PLANT_COST, "Fission Power Plant", 0, FISSION_PLANT_PRODUCTION, 0, 
		ResourceCollection {{Resources::URANIUM, 100}, {Resources::GOLD, 50}},
		"Boosts city productivity by a small amount")
	},

	{
		BuildingType::TypeIdentifier::FUSION_PLANT,
		new BuildingTypeImpl<FusionPlant>(TypeIdentifier::FUSION_PLANT, FUSION_PLANT_COST, "Fusion Power Plant", 0, FUSION_PLANT_PRODUCTION, 0,
		ResourceCollection {{Resources::URANIUM, 200}, {Resources::NANOMATERIAL, 100}, {Resources::GOLD, 75}},
		"Boosts city productivity by a large amount")
	},
	
	{
		BuildingType::TypeIdentifier::RESEARCH_FACILITY,
		new BuildingTypeImpl<ResearchFacility>(TypeIdentifier::RESEARCH_FACILITY, RESEARCH_FACILITY_COST, "Research Facility", 0, 0, RESEARCH_FACILITY_SCIENCE,
		ResourceCollection {{Resources::GOLD, 50}, {Resources::TITANIUM, 100}},
		"Boosts research speed")
	},

	{
		BuildingType::TypeIdentifier::FORTRESS,
		new BuildingTypeImpl<Fortress>(TypeIdentifier::FORTRESS, FORTRESS_COST, "Fortress", FORTRESS_ARMOR, 0, 0,
		ResourceCollection {{Resources::TITANIUM, 50}},
		"Increases city health")
	},

	{
		BuildingType::TypeIdentifier::HADRON_COLLIDER,
		new BuildingTypeImpl<HadronCollider>(TypeIdentifier::HADRON_COLLIDER, HADRON_COLLIDER_COST, "Hadron Collider", 0, HADRON_COLLIDER_PRODUCTION, HADRON_COLLIDER_SCIENCE,
		ResourceCollection {{Resources::NANOMATERIAL, 50}, {Resources::GOLD, 50}, {Resources::ALUMINUM, 50}},
		"Boosts city production and research speed")
	}

};
