#pragma once

#include "drawable.h"
#include "planet.h"
#include "planet_update.h"
#include "planet_types.h"
#include "drawable_data.h"
#include "drawable_slot.h"

class DrawablePlanet : public Planet, public Drawable {
public:
	static std::unordered_map<PlanetType, DrawableData>& getDataMap();

	DrawablePlanet(const Planet & planet);
	~DrawablePlanet();
	virtual void update();
	void draw(const Shader &, const Camera &) const;
};