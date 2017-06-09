#pragma once

class BuildingType;

class Building {
public:

protected:
	/**
	Constructor for base building.
	@param production Amount of production this building provides.
	@param research_points Amount of research points this building provides.
	*/
	Building(BuildingType* type);

	int armor;
	int production;
	int research_points;
};