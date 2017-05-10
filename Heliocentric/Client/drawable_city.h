#pragma once

#include "city.h"
#include "drawable.h"

class DrawableCity : public City, public Drawable {
public:
	DrawableCity(const City&);
	virtual void update();
};