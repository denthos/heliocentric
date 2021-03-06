#pragma once

#include "drawable.h"
#include "unit.h"
#include "unit_update.h"
#include "model.h"
#include "selectable.h"
#include "unit_type.h"
#include "particle_system.h"
#include "audio_3d_sound.h"
#include "player_icon.h"

class GUI;
class Client;

struct DrawableUnitData {
	Model* model;
	float scalingFactor;
	PlayerIcon* icon;
};

class DrawableUnit : public Unit, public Drawable, public Selectable {
public:
	DrawableUnit(const Unit & unit, Shader * shader, ParticleSystem* laser, ParticleSystem* explosion, ThreeDSoundSystem* sound_system, PlayerIcon* icon);

	~DrawableUnit();
	virtual void draw(const Camera & camera) const;
	void select(GUI*, Client*);
	void unselect(GUI*, Client*);

	static const std::unordered_map<UnitType::TypeIdentifier, DrawableUnitData>& getDataMap();

	void updateRotationMatrix();
	glm::mat4 getRotationMatrix() const;

	// explosion params
	bool is_exploding;
	float explosion_start_time;

	virtual bool do_animation(const Camera & camera) const;
protected:
	virtual void update();

private:
	DrawableUnitData data;
	PlayerIcon* icon;
	Shader* iconShader;
	glm::mat4 rotation_matrix;
	glm::vec3 old_orientation;
	ParticleSystem* laser;
	ParticleSystem* explosion;
	float explosion_counter;
	glm::vec3 laser_offset;
	Audio3DSound* shoot_sound;
	Audio3DSound* explode_sound;
	const float pi = glm::pi<float>();
	bool glow;


	bool client_oldisattacking;
	bool justbeganattack;
};
