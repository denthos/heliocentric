#pragma once

#include "drawable.h"
#include "unit.h"
#include "unit_update.h"
#include "drawable_data.h"
#include "model.h"
#include "particle_system.h"
class DrawableUnit : public Unit, public Drawable {
public:
	DrawableUnit(const Unit & unit, Model* spaceship, ParticleSystem* laser);
	~DrawableUnit();
	virtual void update();
	virtual void draw(const Shader & shader, const Camera & camera) const;

private:
	ParticleSystem* laser;
};