#pragma once

#include "drawable.h"
#include "unit.h"
#include "unit_update.h"
#include "drawable_data.h"

class DrawableUnit : public Unit, public Drawable {
public:
	DrawableUnit(const Unit & unit);
	~DrawableUnit();
	virtual void update();
};