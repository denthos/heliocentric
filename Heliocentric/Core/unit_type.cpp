#include "unit_type.h"
#include "basic_unit.h"
#include "heavy_unit.h"

UnitType* UnitType::getByIdentifier(TypeIdentifier identifier) {
	return unittypeMap[identifier];
}

std::unordered_map<UnitType::TypeIdentifier, UnitType*> UnitType::unittypeMap = {
	{
		UnitType::TypeIdentifier::BASIC_UNIT, 
		new UnitTypeImpl<BasicUnit>(TypeIdentifier::BASIC_UNIT, ResourceCollection {{Resources::ALUMINUM, 15}}, 100, "Basic Unit", 400)
	},

	{
		UnitType::TypeIdentifier::HEAVY_UNIT, 
		new UnitTypeImpl<HeavyUnit>(TypeIdentifier::HEAVY_UNIT, ResourceCollection {{Resources::ALUMINUM, 50}, {Resources::TITANIUM, 20}}, 200, "Heavy Unit", 800)
	}
};

