#include "slot.h"

Slot::Slot(UID id, glm::vec3 pos) : GameObject(id, pos) {}

bool Slot::is_occupied() {
	return (this->get_player() != NULL);
}