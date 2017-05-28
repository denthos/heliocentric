#include "unit_type.h"

UnitType::UnitType(TypeIdentifier ident, ResourceCollection buildRequirements, int buildTime, std::string typeName) :
identifier(ident), buildRequirements(buildRequirements), buildTime(buildTime), typeName(typeName) {}

UnitType::UnitType() : UnitType(TypeIdentifier::BASIC_UNIT, ResourceCollection(), 0, "Default") {}

bool UnitType::hasBuildRequirements(const ResourceCollection& resources) const {
	for (auto& resource_pair : this->buildRequirements) {

		int required_resources = resource_pair.second;
		auto resource_iter = resources.find(resource_pair.first);

		if (resource_iter == resources.end() && required_resources > 0) {
			return false;
		}

		if (resource_iter != resources.end() && resource_iter->second < required_resources) {
			return false;
		}
	}

	return true;
}

const std::string& UnitType::getTypeName() const {
	return this->typeName;
}

UnitType::TypeIdentifier UnitType::getIdentifier() const {
	return this->identifier;
}


UnitType* UnitType::getByIdentifier(TypeIdentifier identifier) {
	return &unittype_map[identifier];
}

std::unordered_map<UnitType::TypeIdentifier, UnitType> UnitType::unittype_map = {
	{UnitType::TypeIdentifier::BASIC_UNIT, UnitType(TypeIdentifier::BASIC_UNIT, ResourceCollection {{Resources::ALUMINUM, 50}}, 100, "Basic Unit")}
};