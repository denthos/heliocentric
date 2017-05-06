#pragma once

#include "slot.h"
#include "drawable.h"

class DrawableSlot : public Slot, public Drawable {
public:
	DrawableSlot(const Slot&, Planet* planet);
	virtual void update();
};