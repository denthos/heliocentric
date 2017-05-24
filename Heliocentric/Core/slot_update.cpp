#include "slot_update.h"

SlotUpdate::SlotUpdate(UID id, Resources::Type resource, int new_amount) :
	id(id), resource(resource), resource_amount(new_amount) {}

void SlotUpdate::apply(Slot* slot) {
	slot->changeResourceCount(resource, resource_amount);
}