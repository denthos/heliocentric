#include "slot_update.h"

SlotUpdate::SlotUpdate(UID id, float x, float y, float z) : GameObjectUpdate::GameObjectUpdate(id, x, y, z) {

}

void SlotUpdate::apply(GameObject* obj) {
	GameObjectUpdate::apply(obj);

	Slot* slot = reinterpret_cast<Slot*>(obj);
}