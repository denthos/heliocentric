#pragma once
#include "buildable.h"
#include <unordered_map>

class BuildingType : public Buildable {
public:
	enum TypeIdentifier {
		FISSION_PLANT,
		FUSION_PLANT
	};

	const static TypeIdentifier FIRST = FISSION_PLANT; // should always be the first in enum
	const static int NUM_TYPES = 2;

	static BuildingType* getByIdentifier(TypeIdentifier);

	virtual void createBuilding() = 0;

	virtual const std::string& getTypeName() const = 0;
	virtual TypeIdentifier getIdentifier() const = 0;

private:
	static std::unordered_map<BuildingType::TypeIdentifier, BuildingType*> buildingTypeMap;
};

template <typename BuildingClass>
class BuildingTypeImpl : public BuildingType {
public:

};