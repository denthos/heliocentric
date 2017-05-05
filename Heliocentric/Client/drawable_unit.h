#pragma once

#include "drawable.h"
#include "unit.h"
#include "unit_update.h"
#include "drawable_data.h"
#include "model.h"

class DrawableUnit : public Unit, public Drawable {
public:
	DrawableUnit(const Unit & unit, Model* spaceship);
	~DrawableUnit();
	virtual void update();
};