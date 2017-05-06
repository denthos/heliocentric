#pragma once

#include "slot.h"
#include "drawable.h"

class DrawableSlot : public Slot, public Drawable {
public:
	DrawableSlot(const Slot&, Planet* planet);
	virtual void update();
	virtual void draw(const Shader &, const Camera &) const;
	virtual void foundCity(Player* owner, int att, int def, int range, int heal, int pr, int pop);
};