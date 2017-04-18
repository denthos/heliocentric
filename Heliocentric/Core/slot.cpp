#include "slot.h"

Slot::Slot(int slot_ID, bool occupied, glm::vec3 pos) : ID(slot_ID) {
	position = pos;
}

int Slot::get_slot_ID() {
	return ID;
}

bool Slot::is_occupied() {
	return (this->get_player() != NULL);
}

void Slot::assign_player(Player* owner) {
	player = owner;
}