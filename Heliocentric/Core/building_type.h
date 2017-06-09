#pragma once
#include "buildable.h"
#include "identifiable.h"
#include <unordered_map>
#include "building.h"

#define FISSION_PLANT_PRODUCTION 2
#define FUSION_PLANT_PRODUCTION 5
#define RESEARCH_FACILITY_SCIENCE 5

class BuildingType : public Buildable {
public:
	enum TypeIdentifier {
		FISSION_PLANT,
		FUSION_PLANT,
		RESEARCH_FACILITY
	};

	const static TypeIdentifier FIRST = FISSION_PLANT; // should always be the first in enum
	const static int NUM_TYPES = 3;

	BuildingType(BuildType buildType, int productionCost);

	static BuildingType* getByIdentifier(TypeIdentifier);

	virtual const std::string& getTypeName() const = 0;
	virtual TypeIdentifier getIdentifier() const = 0;

	virtual int getProduction() const = 0;
	virtual int getResearchPoints() const = 0;

private:
	static std::unordered_map<BuildingType::TypeIdentifier, BuildingType*> buildingTypeMap;
};

template <typename BuildingClass>
class BuildingTypeImpl : public BuildingType {
public:
	BuildingTypeImpl(TypeIdentifier ident, int productionCost, std::string typeName, int production, int research_points) :
		BuildingType(Buildable::BuildType::BUILDING, productionCost), identifier(ident), typeName(typeName), production(production), research_points(research_points) {}

	int getProductionCost() const {
		return this->productionCost;
	}

	const std::string& getTypeName() const {
		return this->typeName;
	}

	TypeIdentifier getIdentifier() const {
		return this->identifier;
	}

	int getProduction() const {
		return this->production;
	}

	int getResearchPoints() const {
		return this->research_points;
	}

private:
	int production;
	int research_points;

	std::string typeName;

	TypeIdentifier identifier;
};