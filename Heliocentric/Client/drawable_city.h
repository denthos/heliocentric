#pragma once

#include "city.h"
#include "drawable.h"
#include "selectable.h"
#include "player_icon.h"

class DrawableSlot;

class DrawableCity : public City, public Drawable, public Selectable {
	friend class DrawableSlot;

private:
	DrawableSlot* drawable_slot;
	PlayerIcon* icon;

public:
	DrawableCity(const City&, Shader * shader, PlayerIcon * icon);
	virtual void draw(const Camera & camera) const;
	void select(GUI* gui, Client* client);
	void unselect(GUI* gui, Client* client);

protected:
	virtual void update();
};
