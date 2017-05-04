#pragma once

#include "game_object_update.h"
#include "slot.h"

class SlotUpdate : public GameObjectUpdate {
public:
	/**
	Constructor of a slot update.
	@param id UID of the slot to be updated.
	@param x I dunno why we have this.
	@param y I dunno why we have this.
	@param z I dunno why we have this.
	*/
	SlotUpdate(UID, float, float, float);
	void apply(GameObject* obj);

};