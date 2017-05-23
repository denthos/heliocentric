#pragma once

#include "identifiable.h"
#include "resources.h"
#include "slot.h"

class SlotUpdate {
public:
	UID id;

	Resources::Type resource;
	int resource_amount;

	SlotUpdate(UID, Resources::Type, int);
	void apply(Slot* slot);
};