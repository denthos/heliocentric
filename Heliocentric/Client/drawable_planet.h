#pragma once

#include "drawable.h"
#include "planet.h"
#include "planet_update.h"

class DrawablePlanet : public Planet, public Drawable {
public:
	DrawablePlanet(const Planet & planet, Texture texture);
	~DrawablePlanet();
	virtual void update();
};