#pragma once
#include "resources.h"

class UnitType {

public:
	enum TypeIdentifier {
		BASIC_UNIT
	};

	static UnitType* getByIdentifier(TypeIdentifier identifier);

	UnitType();
	UnitType(TypeIdentifier ident, ResourceCollection buildRequirements, int buildTime, std::string typeName);

	bool hasBuildRequirements(const ResourceCollection& resources) const;
	const std::string& getTypeName() const;

	TypeIdentifier getIdentifier() const;

private:
	ResourceCollection buildRequirements;
	int buildTime;

	std::string typeName;

	TypeIdentifier identifier;

	static std::unordered_map<TypeIdentifier, UnitType> unittype_map;
};