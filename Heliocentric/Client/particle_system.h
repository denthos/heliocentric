#pragma once
#include "glm\glm.hpp"
#include "shader.h"
#include "texture.h"
#include "particle_emitter.h"
#include "camera.h"

#define MAX_PARTICLES 10000

class ParticleSystem {
public:
	ParticleSystem(float spawn_rate, int spawns_per_emission, ParticleEmitter* emitter); //init fields
	~ParticleSystem();

	void Update(const Camera &camera);
	void draw(const Shader &shader, const Camera &camera, const glm::mat4 &toWorld);
	//Texture texture = Texture("Textures/fire.png");

	glm::mat4 world_mat;

private:
	int spawns_per_emission;
	ParticleEmitter* emitter;
	//force
	float creationTime, currTime;
	GLuint particles_buffer;

	int max_particles, particle_count;
	float spawn_rate; // new particles per unit time

	Particle particles[MAX_PARTICLES]; //particles in our system
	int lastUsedIndex; //where the last particle that died was?

	//buffers
	GLuint VAO, VBO;

	void genParticleBuffers(); //set up the buffers we need to keep track of particle info
	void updateParticle(Particle &particle, const Camera & camera); //perform particle physics
	
	int findLastUsed();
	void spawn(int spawnIndex);
	void sortParticles();
};