#pragma once

#include "drawable.h"
#include "unit.h"
#include "unit_update.h"
#include "drawable_data.h"
#include "model.h"
#include "selectable.h"

#include "particle_system.h"
#include "shader.h"
class GUI;
class Client;

class DrawableUnit : public Unit, public Drawable, public Selectable {
public:
	DrawableUnit(const Unit & unit, Model* spaceship, Shader* unitShader, ParticleSystem* laser, ParticleSystem* explosion);
	~DrawableUnit();
	virtual void update();

	void select(GUI*, Client*);
	void unselect(GUI*, Client*);
	virtual void draw(const Shader & shader, const Camera & camera) const;
	bool is_exploding;
	void bind_shader(Shader* shader);
	virtual bool do_animation(const Shader & shader, const Camera & camera) const;

private:
	ParticleSystem* laser;
	ParticleSystem* explosion;
	Shader* unitShader;
	glm::vec3 shooting_offset; //offset from center to laser origin
	int explosion_counter;
};