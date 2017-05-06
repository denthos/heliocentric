#include "slot.h"

Slot::Slot(glm::vec3 pos) : GameObject(pos) {}
Slot::Slot(UID id, glm::vec3 pos) : GameObject(id, pos) {}

bool Slot::is_occupied() {
	return (this->get_player() != NULL);
}

void Slot::foundCity(Player* owner, int att, int def, int range, int heal, int pr, int pop) {
	this->city = new City(owner, att, def, range, heal, pr, pop, this);
}