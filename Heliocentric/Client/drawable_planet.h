#pragma once

#include "drawable.h"
#include "planet.h"
#include "planet_update.h"
#include "planet_types.h"
#include "drawable_data.h"
#include "selectable.h"

class DrawablePlanet : public Planet, public Drawable, public Selectable {
public:
	static std::unordered_map<PlanetType, DrawableData>& getDataMap();

	DrawablePlanet(const Planet & planet, Shader * shader, Shader * slotShader);
	~DrawablePlanet();
	virtual void update();
};