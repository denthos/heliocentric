#include "slot_update.h"

void SlotUpdate::apply(GameObject* obj) {
	GameObjectUpdate::apply(obj);

	Slot* slot = reinterpret_cast<Slot*>(obj);
}