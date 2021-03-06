#include "unit_type.h"
#include "basic_unit.h"
#include "heavy_unit.h"

UnitType::UnitType(BuildType buildType, int productionCost) : Buildable(Buildable::BuildType::UNIT, productionCost) {

}

UnitType* UnitType::getByIdentifier(TypeIdentifier identifier) {
	return unittypeMap[identifier];
}

std::unordered_map<UnitType::TypeIdentifier, UnitType*> UnitType::unittypeMap = {
	{
		UnitType::TypeIdentifier::BASIC_UNIT, 
		new UnitTypeImpl<BasicUnit>(TypeIdentifier::BASIC_UNIT, ResourceCollection {{Resources::ALUMINUM, 15}}, 100, "Basic Unit", 400, 100, {},
		"A basic cheap unit with standard stats")
	},

	{
		UnitType::TypeIdentifier::HEAVY_UNIT, 
		new UnitTypeImpl<HeavyUnit>(TypeIdentifier::HEAVY_UNIT, ResourceCollection {{Resources::ALUMINUM, 50}, {Resources::TITANIUM, 20}}, 200, "Heavy Unit", 800, 150, {TECH_3},
		"A unit with a large amount of health and strong attack")
	}
};

