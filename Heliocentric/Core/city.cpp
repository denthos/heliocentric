#include "city.h"
#include <iostream>
#include <string>

City::City(int att, int arm, int heal, int pr, int pop, Slot* assigned_slot) :
	production(pr), population(pop), slot(assigned_slot) {
	combatAttack = att;
	combatDefense = arm;
	health = heal;
}

int City::get_population() {
	return population;
}
void City::set_population(int new_pop) {
	population = new_pop;
}

Slot* City::get_slot() {
	return slot;
}

void City::print() {
	//TO DO
}

std::ostream& City::operator<< (std::ostream & out) {
	out << "This city has the following stats: " << "attack " << combatAttack << ", armor " << combatDefense << ", health " << health << ", production rate " << production << ", and population of " << population << " people. ";
	if (slot != NULL)
		out << "City is located in " << slot->get_slot_ID() << ", owned by player " << slot->get_player()->get_name() << std::endl;
	return out;
}