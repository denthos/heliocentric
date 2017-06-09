#pragma once

#include "drawable.h"
#include "planet.h"
#include "planet_update.h"
#include "planet_types.h"
#include "selectable.h"

struct DrawablePlanetData {
	const Texture* texture;
};

class DrawablePlanet : public Planet, public Drawable, public Selectable {
public:
	static std::unordered_map<PlanetType, DrawablePlanetData>& getDataMap();

	DrawablePlanet(const Planet & planet, Shader * shader, Shader * slotShader);
	~DrawablePlanet();
protected:
	virtual void update();
};