#pragma once

#include "game_object_update.h"
#include "slot.h"

class SlotUpdate : public GameObjectUpdate {
public:
	void apply(GameObject* obj);

};
