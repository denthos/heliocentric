#pragma once

#include "drawable.h"
#include "unit.h"
#include "unit_update.h"

class DrawableUnit : public Unit, public Drawable {
public:
	DrawableUnit(const Unit & unit, Texture texture);
	~DrawableUnit();
	virtual void update();
};