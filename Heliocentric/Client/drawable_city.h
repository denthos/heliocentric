#pragma once

#include "city.h"
#include "drawable.h"

class DrawableSlot;

class DrawableCity : public City, public Drawable {
private:
	DrawableSlot* drawable_slot;

public:
	DrawableCity(const City&);
	virtual void update();
};