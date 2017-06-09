#pragma once

#include "slot.h"
#include "drawable.h"
#include "selectable.h"

class DrawablePlanet;
class DrawableCity;

class DrawableSlot : public Slot, public Drawable, public Selectable {

	friend class DrawablePlanet;

public:
	DrawableSlot(const Slot&, DrawablePlanet* planet, Shader * shader);
	virtual void draw(const Camera &) const;
	void select(GUI* gui, Client* client);
	void unselect(GUI* gui, Client* client);
	Selectable* getSelection();
protected:
	virtual void update();
};