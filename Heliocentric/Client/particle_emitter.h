#pragma once
#include "glm\glm.hpp"

#include "glm\gtx\spline.hpp"
#include <glad\glad.h>

struct Particle {
	glm::vec3 pos, 
		velocity;
	
	glm::vec4 color;
	float size;
	float life;
	float camDist;

	//true if particle is further from camera
	bool operator < (Particle &p);
	bool isAlive();

	Particle();
};

//default particle emitter: shows exploding sphere yellow in center and red outside
class ParticleEmitter {
public:
	virtual void spawn(Particle &particle );
	virtual void simulate(Particle & particle, float deltaTime);
};