#include "slot.h"

Slot::Slot(UID id, glm::vec3 pos) : GameObject(id) {
	position = pos;
	player = NULL;
}

bool Slot::is_occupied() {
	return (this->get_player() != NULL);
}