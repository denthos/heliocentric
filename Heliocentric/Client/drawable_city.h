#pragma once

#include "city.h"
#include "slot.h"
#include "drawable.h"

class DrawableCity : public City, public Drawable {
public:
	DrawableCity(const City&, Slot* slot);
	virtual void update();
};