#pragma once
#include "buildable.h"
#include "identifiable.h"
#include <unordered_map>
#include "building.h"

class BuildingType : public Buildable {
public:
	enum TypeIdentifier {
		FISSION_PLANT,
		FUSION_PLANT,
		RESEARCH_FACILITY
	};

	const static TypeIdentifier FIRST = FISSION_PLANT; // should always be the first in enum
	const static int NUM_TYPES = 3;

	static BuildingType* getByIdentifier(TypeIdentifier);

	virtual const std::string& getTypeName() const = 0;
	virtual TypeIdentifier getIdentifier() const = 0;

private:
	static std::unordered_map<BuildingType::TypeIdentifier, BuildingType*> buildingTypeMap;
};

template <typename BuildingClass>
class BuildingTypeImpl : public BuildingType {
public:
	BuildingTypeImpl(TypeIdentifier ident, int productionCost, std::string typeName) :
		Buildable(Buildable::BuildType::BUILDING, productionCost), identifier(ident), typeName(typeName) {}

	int getProductionCost() const {
		return this->productionCost;
	}

	const std::string& getTypeName() const {
		return this->typeName;
	}

	TypeIdentifier getIdentifier() const {
		return this->identifier;
	}

private:
	std::string typeName;

	TypeIdentifier identifier;
};