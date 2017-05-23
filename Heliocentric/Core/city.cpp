#include "city.h"
#include <iostream>
#include <string>

City::City(Player* owner, Attack* attack, int def, int heal, int pr, int pop, Slot* assigned_slot, std::string name) :
	AttackableGameObject(assigned_slot->get_position(), owner, attack, def, heal), production(pr), population(pop), slot(assigned_slot), name(name) {}


City::City(UID id, Player* owner, Attack* attack, int def, int heal, int pr, int pop, Slot* assigned_slot, std::string name) :
	AttackableGameObject(id, assigned_slot->get_position(), owner, attack, def, heal), production(pr), population(pop), slot(assigned_slot), name(name) {}

int City::get_population() {
	return population;
}

void City::set_population(int new_pop) {
	population = new_pop;
}

Slot* City::get_slot() {
	return slot;
}

glm::vec3 City::get_position() const {
	return slot->get_position();
}

std::string City::getName() const {
	return name;
}
