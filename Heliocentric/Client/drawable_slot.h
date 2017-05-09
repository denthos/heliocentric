#pragma once

#include "slot.h"
#include "drawable.h"

class DrawablePlanet;
class DrawableCity;

class DrawableSlot : public Slot, public Drawable {
public:
	DrawableSlot(const Slot&, DrawablePlanet* planet);
	virtual void update();
	virtual void draw(const Shader &, const Camera &) const;
};