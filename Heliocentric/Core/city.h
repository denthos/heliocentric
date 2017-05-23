#pragma once

#define CITY_NAME_SIZE 16

#include "attackable_game_object.h"
#include "slot.h"
#include "player.h"

class CityUpdate;

class City : public AttackableGameObject {
public:
	friend CityUpdate;

	City(Player* owner, Attack* attack, int def, int heal, int pr, int pop, Slot* assigned_slot, std::string name);
	City(UID id, Player* owner, Attack* attack, int def, int heal, int pr, int pop, Slot* assigned_slot, std::string name);

	int get_population();
	void set_population(int new_pop);
	Slot* get_slot();
	glm::vec3 get_position() const;

	std::string getName() const;

private:
	std::string name;
	int production;
	int population;
	Slot* slot;

protected:
	// TODO: Override these combat hooks
	virtual void handle_out_of_range(AttackableGameObject* opponent) {}
	virtual void handle_defeat(AttackableGameObject* opponent) {}
	virtual void handle_victory(AttackableGameObject* opponent) {}
	virtual void handle_counter(AttackableGameObject* opponent) {}
};