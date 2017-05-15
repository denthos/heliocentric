#pragma once

#include "drawable.h"
#include "unit.h"
#include "unit_update.h"
#include "drawable_data.h"
#include "model.h"
#include "particle_system.h"
#include "shader.h"
class DrawableUnit : public Unit, public Drawable {
public:
	DrawableUnit(const Unit & unit, Model* spaceship, Shader* unitShader, ParticleSystem* laser, ParticleSystem* explosion);
	~DrawableUnit();
	virtual void update();
	virtual void draw(const Shader & shader, const Camera & camera) const;

private:
	ParticleSystem* laser;
	ParticleSystem* explosion;
	Shader* unitShader;
	glm::vec3 shooting_offset; //offset from center to laser origin
};