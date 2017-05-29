#pragma once

#include "drawable.h"
#include "unit.h"
#include "unit_update.h"
#include "drawable_data.h"
#include "model.h"
#include "selectable.h"

class GUI;
class Client;

class DrawableUnit : public Unit, public Drawable, public Selectable {
public:
	DrawableUnit(const Unit & unit, Model* spaceship, Shader * shader);
	~DrawableUnit();
	virtual void update();

	void select(GUI*, Client*);
	void unselect(GUI*, Client*);
};