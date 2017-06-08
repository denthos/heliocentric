#pragma once
#include "resources.h"
#include "identifiable.h"
#include <glm/vec3.hpp>
#include <memory>
#include "unit.h"

class Player;
class UnitManager;

class UnitType{
public:
	enum TypeIdentifier {
		BASIC_UNIT,
		HEAVY_UNIT
	};

	const static TypeIdentifier FIRST = BASIC_UNIT;
	const static int NUM_TYPES = 2;

	static UnitType* getByIdentifier(TypeIdentifier);

	virtual std::shared_ptr<Unit> createUnit(glm::vec3 position, Player* owner, UnitManager* manager) = 0;
	virtual std::shared_ptr<Unit> createUnit(UID id, glm::vec3 position, Player* owner, UnitManager* manager) = 0;


	virtual bool hasBuildRequirements(const ResourceCollection& resources) const = 0;
	virtual bool hasTechRequirements(const TechTree& tree) const = 0;
	virtual const ResourceCollection& getBuildRequirements() const = 0;

	virtual int getBuildTime() const = 0;

	virtual const std::string& getTypeName() const = 0;
	virtual TypeIdentifier getIdentifier() const = 0;

	virtual int getBaseHealth() const = 0;

private:
	static std::unordered_map<UnitType::TypeIdentifier, UnitType*> unittypeMap;
};

template <typename UnitClass>
class UnitTypeImpl : public UnitType {
public:
	UnitTypeImpl(TypeIdentifier ident, ResourceCollection buildRequirements, int buildTime, std::string typeName, int baseHealth, std::vector<int> required_techs) :
		identifier(ident), buildRequirements(buildRequirements), buildTime(buildTime), typeName(typeName), baseHealth(baseHealth), requiredTechs(required_techs) {}

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

	virtual bool hasTechRequirements(const TechTree& tree) const {
		for (int required_tech_id : this->requiredTechs) {
			if (!tree.getTechById(required_tech_id)->hasResearched()) {
				return false;
			}
		}

		return true;
	}

	const ResourceCollection& getBuildRequirements() const {
		return this->buildRequirements;
	}

	int getBuildTime() const {
		return this->buildTime;
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
	std::vector<int> requiredTechs;
	int buildTime;
	int baseHealth;

	std::string typeName;

	TypeIdentifier identifier;

};
