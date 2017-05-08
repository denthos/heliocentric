#include "city.h"
#include <iostream>
#include <string>

City::City(Player* owner, int att, int def, int range, int heal, int pr, int pop, Slot* assigned_slot) :
	AttackableGameObject(assigned_slot->get_absolute_position(), owner, att, def, range, heal), production(pr), population(pop), slot(assigned_slot) {}


City::City(UID id, Player* owner, int att, int def, int range, int heal, int pr, int pop, Slot* assigned_slot) :
	AttackableGameObject(id, assigned_slot->get_absolute_position(), owner, att, def, range, heal), production(pr), population(pop), slot(assigned_slot) {}

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
	return slot->get_absolute_position();
}