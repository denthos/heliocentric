#include "city.h"
#include <iostream>
#include <string>

City::City(int att, int def, int range, int heal, int pr, int pop, Slot* assigned_slot) :
	AttackableGameObject(att, def, range, heal), production(pr), population(pop), slot(assigned_slot) {
	position = slot->get_position();
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

glm::vec3 City::get_position() {
	return slot->get_position();
}