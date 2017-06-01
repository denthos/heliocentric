#pragma once

#include "drawable.h"
#include "unit.h"
#include "unit_update.h"
#include "model.h"
#include "selectable.h"
#include "unit_type.h"

class GUI;
class Client;

struct DrawableUnitData {
	Model* model;
	float scalingFactor;
};

class DrawableUnit : public Unit, public Drawable, public Selectable {
public:
	DrawableUnit(const Unit & unit, Shader * shader);

	~DrawableUnit();
	virtual void update();
	virtual void draw(const Camera & camera) const;
	void select(GUI*, Client*);
	void unselect(GUI*, Client*);

	static const std::unordered_map<UnitType::TypeIdentifier, DrawableUnitData>& getDataMap();

private:
	DrawableUnitData data;
	bool glow;
};
