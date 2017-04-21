#pragma once

#include "attackable.h"
#include "slot.h"
#include "player.h"

class CityUpdate;

class City : public Attackable {
public:
	friend CityUpdate;
	City(int att, int arm, int heal, int pr, int pop, Slot* assigned_slot);
	int get_population();
	void set_population(int new_pop);
	Slot* get_slot();
	void print();
	std::ostream& operator<<(std::ostream&);

private:
	int production;
	int population;
	Slot* slot;
};