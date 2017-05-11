#pragma once

#include "particle_emitter.h"
class LaserEmitter : public ParticleEmitter {
public:
	virtual void spawn(Particle &particle);
	virtual void simulate(Particle & particle, float deltaTime);
};