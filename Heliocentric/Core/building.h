#pragma once

class Building {
public:

protected:
	/**
	Constructor for base building.
	@param production Amount of production this building provides.
	@param research_points Amount of research points this building provides.
	*/
	Building(int production, int research_points);

	int production;
	int research_points;
};