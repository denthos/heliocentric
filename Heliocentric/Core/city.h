#pragma once

#include "attackable_game_object.h"
#include "slot.h"
#include "player.h"

struct CityUpdate;

class City : public AttackableGameObject {
public:
	friend CityUpdate;
	City(int att, int arm, int heal, int pr, int pop, Slot* assigned_slot);
	int get_population();
	void set_population(int new_pop);
	Slot* get_slot();

private:
	int production;
	int population;
	Slot* slot;
};