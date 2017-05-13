#include "particle_emitter.h"

void ParticleEmitter::spawn(Particle & particle)
{
	//get random sphere coordinates
	float inclination = glm::radians((float)(rand() % 180));
	float azimuth = glm::radians((float)(rand() % 360));
	float radius = 1.0f;

	float sInclination = sinf(inclination);

	float x = sInclination * cosf(azimuth);
	float y = sInclination * sinf(azimuth);
	float z = cosf(inclination);

	glm::vec3 sVec(x, y, z); //random sphere coordinate

							 //random color values to range from yellow to red
	GLfloat rAlpha = (((rand() % 100)) / 100.0f);
	GLfloat rRed = 0.7f + (((rand() % 100)) / 100.0f);
	GLfloat rGreen = (((rand() % 55)) / 100.0f);
	GLfloat rBlue = (((rand() % 25)) / 100.0f);


	particle.pos = sVec * radius;
	particle.size = 0.3f; //todo: randomize
	particle.color = glm::vec4(rRed, rGreen, rBlue, rAlpha);
	particle.life = 1.0f; //TODO randomize
	particle.velocity = sVec * 5.0f;
}

void ParticleEmitter::simulate(Particle & particle, float deltaTime)
{
	float time = 1 - particle.life; //used for interpolation purposes


	particle.pos += particle.velocity * deltaTime;
	particle.size *= 0.99f;

	//update color
	particle.color = glm::mix(glm::vec4(1.0f, 0.25f * time, 0.05f, 0.25f), particle.color, particle.life);

}
