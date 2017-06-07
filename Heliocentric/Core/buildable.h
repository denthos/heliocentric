#pragma once

class Buildable {
public:
	Buildable(int productionCost);

	virtual int getProductionCost() const = 0;

protected:
	int productionCost;
};