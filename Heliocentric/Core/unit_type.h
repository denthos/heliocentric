#pragma once
#include "buildable.h"
#include "resources.h"
#include "identifiable.h"
#include <glm/vec3.hpp>
#include <memory>
#include "unit.h"

class Player;
class UnitManager;

class UnitType : public Buildable {
public:
	enum TypeIdentifier {
		BASIC_UNIT,
		HEAVY_UNIT
	};

	const static TypeIdentifier FIRST = BASIC_UNIT; // should always be the first in enum
	const static int NUM_TYPES = 2;

	UnitType(BuildType buildType, int productionCost);

	static UnitType* getByIdentifier(TypeIdentifier);

	virtual std::shared_ptr<Unit> createUnit(glm::vec3 position, Player* owner, UnitManager* manager) = 0;
	virtual std::shared_ptr<Unit> createUnit(UID id, glm::vec3 position, Player* owner, UnitManager* manager) = 0;


	virtual bool hasBuildRequirements(const ResourceCollection& resources) const = 0;
	virtual const ResourceCollection& getBuildRequirements() const = 0;

	virtual const std::string& getTypeName() const = 0;
	virtual TypeIdentifier getIdentifier() const = 0;

	virtual int getBaseHealth() const = 0;

private:
	static std::unordered_map<UnitType::TypeIdentifier, UnitType*> unittypeMap;
};

template <typename UnitClass>
class UnitTypeImpl : public UnitType {
public:
	UnitTypeImpl(TypeIdentifier ident, ResourceCollection buildRequirements, int productionCost, std::string typeName, int baseHealth) :
		UnitType(Buildable::BuildType::UNIT, productionCost), identifier(ident), buildRequirements(buildRequirements), typeName(typeName), baseHealth(baseHealth) {}

	std::shared_ptr<Unit> createUnit(glm::vec3 position, Player* owner, UnitManager* manager) {
		return std::make_shared<UnitClass>(position, owner, manager, this);
	}

	std::shared_ptr<Unit> createUnit(UID id, glm::vec3 position, Player* owner, UnitManager* manager) {
		return std::make_shared<UnitClass>(id, position, owner, manager, this);
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

	int getProductionCost() const {
		return this->productionCost;
	}

	int getBaseHealth() const {
		return this->baseHealth;
	}

	const std::string& getTypeName() const {
		return this->typeName;
	}

	TypeIdentifier getIdentifier() const {
		return this->identifier;
	}

private:
	ResourceCollection buildRequirements;
	int baseHealth;

	std::string typeName;

	TypeIdentifier identifier;

};
