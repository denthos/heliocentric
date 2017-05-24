#pragma once

#include "city.h"
#include "drawable.h"
#include "selectable.h"

class DrawableSlot;

class DrawableCity : public City, public Drawable, public Selectable {
private:
	DrawableSlot* drawable_slot;

public:
	DrawableCity(const City&);
	virtual void update();
	void select(GUI* gui, Client* client);
	void unselect(GUI* gui, Client* client);
};
