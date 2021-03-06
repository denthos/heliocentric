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
	virtual bool hasTechRequirements(const TechTree& tree) const = 0;
	virtual const ResourceCollection& getBuildRequirements() const = 0;
	virtual const std::string& getDescription() const = 0;

	virtual const std::string& getTypeName() const = 0;
	virtual TypeIdentifier getIdentifier() const = 0;

	virtual int getBaseHealth() const = 0;
	virtual int getBaseDefense() const = 0;

private:
	static std::unordered_map<UnitType::TypeIdentifier, UnitType*> unittypeMap;
};

template <typename UnitClass>
class UnitTypeImpl : public UnitType {
public:
	UnitTypeImpl(TypeIdentifier ident, ResourceCollection buildRequirements, int productionCost, std::string typeName, int baseHealth, int baseDefense, std::vector<int> required_techs, std::string desc) :
		UnitType(Buildable::BuildType::UNIT, productionCost), identifier(ident), buildRequirements(buildRequirements), typeName(typeName), baseHealth(baseHealth), baseDefense(baseDefense), requiredTechs(required_techs),
		description(desc) {}

	std::shared_ptr<Unit> createUnit(glm::vec3 position, Player* owner, UnitManager* manager) {
		std::shared_ptr<UnitClass> unit = std::make_shared<UnitClass>(position, owner, manager, this);
		applyChangesToUnit(unit, owner);
		return unit;
	}

	std::shared_ptr<Unit> createUnit(UID id, glm::vec3 position, Player* owner, UnitManager* manager) {
		std::shared_ptr<UnitClass> unit = std::make_shared<UnitClass>(id, position, owner, manager, this);
		applyChangesToUnit(unit, owner);
		return unit;
	}

	void applyChangesToUnit(std::shared_ptr<Unit> unit, Player* owner) {
		/* Let's see if the user researched increased armor */
		const Technology* armor_tech = owner->getTechTree().getTechById(TECH_1);
		const Technology* advanced_warfare_tech = owner->getTechTree().getTechById(TECH_4);

		if (armor_tech && armor_tech->hasResearched()) {
			unit->set_combat_defense(unit->get_combat_defense() + 10);
		}

		if (advanced_warfare_tech && advanced_warfare_tech->hasResearched()) {
			unit->set_combat_defense(unit->get_combat_defense() + 20);
			unit->set_movement_speed_max(unit->get_movement_speed_max() + 1.0f);
			unit->set_health(unit->get_health() + 200);
		}
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

	virtual bool hasTechRequirements(const TechTree& tree) const {
		for (int required_tech_id : this->requiredTechs) {
			if (!tree.getTechById(required_tech_id)->hasResearched()) {
				return false;
			}
		}

		return true;
	}


	int getProductionCost() const {
		return this->productionCost;
	}


	const std::string& getDescription() const {
		return this->description;
	}

	int getBaseHealth() const {
		return this->baseHealth;
	}

	int getBaseDefense() const {
		return this->baseDefense;
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
	int baseHealth;
	int baseDefense;

	std::string typeName;
	std::string description;

	TypeIdentifier identifier;

};
