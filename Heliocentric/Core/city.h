#pragma once

#include "attackable_game_object.h"
#include "slot.h"
#include "player.h"

class CityUpdate;

class City : public AttackableGameObject {
public:
	friend CityUpdate;

	City(Player* owner, int att, int def, int range, int heal, int pr, int pop, Slot* assigned_slot);
	City(UID id, Player* owner, int att, int def, int range, int heal, int pr, int pop, Slot* assigned_slot);

	int get_population();
	void set_population(int new_pop);
	Slot* get_slot();
	const glm::vec3& get_position();

private:
	int production;
	int population;
	Slot* slot;
};