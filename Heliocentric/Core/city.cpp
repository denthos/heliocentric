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