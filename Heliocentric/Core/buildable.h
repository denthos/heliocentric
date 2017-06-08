#pragma once

class Buildable {
public:
	enum BuildType { BUILDING, UNIT };

	Buildable(BuildType buildType, int productionCost);

	virtual int getProductionCost() const = 0;
	BuildType getBuildType();

protected:
	BuildType buildType;
	int productionCost;
};