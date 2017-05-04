#pragma once

#include "drawable.h"
#include "planet.h"
#include "planet_update.h"
#include "planet_types.h"
#include "drawable_data.h"

class DrawablePlanet : public Planet, public Drawable {
public:
	static std::unordered_map<PlanetType, DrawableData>& getDataMap();

	DrawablePlanet(const Planet & planet);
	~DrawablePlanet();
	virtual void update();
};