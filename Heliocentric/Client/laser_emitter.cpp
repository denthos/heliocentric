#include "laser_emitter.h"

void LaserEmitter::spawn(Particle & particle)
{
	particle.pos = glm::vec3(0.0f);
	particle.size = 1.0f; //todo: randomize
	particle.color = glm::vec4(0.0f, 0.2f, 1.0f, 0.05f);
	particle.life = 0.70f; //TODO randomize
	particle.velocity = glm::vec3(10.0f , 0.0f, 0.0f);
}

void LaserEmitter::simulate(Particle & particle, float deltaTime)
{
	float time = 1 - particle.life;
	
	particle.pos += particle.velocity * deltaTime;
	particle.color = glm::mix(glm::vec4(0.0, 0.0f , 0.75f, 1.0f), particle.color, particle.life);
}
