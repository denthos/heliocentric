#pragma once
#include "buildable.h"
#include "identifiable.h"
#include "resources.h"
#include <unordered_map>
#include "building.h"

#define FISSION_PLANT_PRODUCTION 2
#define FISSION_PLANT_COST 100
#define FUSION_PLANT_PRODUCTION 5
#define FUSION_PLANT_COST 200
#define RESEARCH_FACILITY_SCIENCE 1
#define RESEARCH_FACILITY_COST 100
#define FORTRESS_ARMOR 20
#define FORTRESS_COST 150

class BuildingType : public Buildable {
public:
	enum TypeIdentifier {
		FISSION_PLANT,
		FUSION_PLANT,
		RESEARCH_FACILITY,
		HADRON_COLLIDER,
		FORTRESS
	};

	const static TypeIdentifier FIRST = FISSION_PLANT; // should always be the first in enum
	const static int NUM_TYPES = 3;

	BuildingType(BuildType buildType, int productionCost);

	static BuildingType* getByIdentifier(TypeIdentifier);

	virtual const std::string& getTypeName() const = 0;
	virtual TypeIdentifier getIdentifier() const = 0;

	virtual int getArmor() const = 0;
	virtual int getProduction() const = 0;
	virtual int getResearchPoints() const = 0;
	virtual bool hasBuildRequirements(const ResourceCollection& resources) const = 0;
	virtual const ResourceCollection& getBuildRequirements() const = 0;

private:
	static std::unordered_map<BuildingType::TypeIdentifier, BuildingType*> buildingTypeMap;
};

template <typename BuildingClass>
class BuildingTypeImpl : public BuildingType {
public:
	BuildingTypeImpl(TypeIdentifier ident, int productionCost, std::string typeName, int armor, int production, int research_points, ResourceCollection build_req) :
		BuildingType(Buildable::BuildType::BUILDING, productionCost), identifier(ident), typeName(typeName), armor(armor), production(production), research_points(research_points),
		buildRequirements(build_req)
		{}

	int getProductionCost() const {
		return this->productionCost;
	}

	const std::string& getTypeName() const {
		return this->typeName;
	}

	TypeIdentifier getIdentifier() const {
		return this->identifier;
	}

	int getArmor() const {
		return this->armor;
	}

	int getProduction() const {
		return this->production;
	}

	int getResearchPoints() const {
		return this->research_points;
	}

	bool hasBuildRequirements(const ResourceCollection& resources) const {
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

	const ResourceCollection& getBuildRequirements() const {
		return this->buildRequirements;
	}

private:
	int armor;
	int production;
	int research_points;

	std::string typeName;

	TypeIdentifier identifier;
	ResourceCollection buildRequirements;
};
