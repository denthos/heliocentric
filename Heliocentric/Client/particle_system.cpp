#include "particle_system.h"
#include "client.h"
#define DEFAULT_SPAWN_RATE  20

ParticleSystem::ParticleSystem(float spawn_rate, int spawns_per_emission, ParticleEmitter* emitter)
{
	this->max_particles  = MAX_PARTICLES;
	this->spawn_rate = spawn_rate;
	this->emitter = emitter; //default emitter
	this->spawns_per_emission = spawns_per_emission;
	

	lastUsedIndex = 0;
	particle_count = 0;

	genParticleBuffers();

	for (int i = 0; i < max_particles; i++) {

		particles[i] =(Particle());
		
	}

	//create first particle
	spawn(findLastUsed());
	particle_count++;
	creationTime = currTime = (float)glfwGetTime();

}

ParticleSystem::~ParticleSystem()
{
}



void ParticleSystem::Update(const Camera &camera)
{
	//spawn new particles
	if ((glfwGetTime() - creationTime) > spawn_rate) {
		for (int i = 0; i < spawns_per_emission; i++) {
			int spawnIndex = findLastUsed();
			spawn(spawnIndex);
			particle_count++;
		}
		

		creationTime = (float)glfwGetTime();
	}

	//simulate particles
	for (int i = 0; i < max_particles; i++) {
		updateParticle(particles[i], camera);
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, particles_buffer);
	glBufferData(GL_ARRAY_BUFFER, max_particles * sizeof(Particle), NULL, GL_STREAM_DRAW); 
	glBufferSubData(GL_ARRAY_BUFFER, 0, particle_count * sizeof(Particle), particles);

}

void ParticleSystem::draw(const Shader &shader, const Camera &camera, const glm::mat4 &toWorld)
{
	sortParticles();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	shader.bind();
	GLuint shaderID = shader.getPid();

	glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, &camera.perspective[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, &camera.view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, &toWorld[0][0]);


	
	
	//bind texture
	//texture.bind();
	glBindVertexArray(VAO);
	
	glDrawArraysInstanced(GL_POINTS, 0, 1, particle_count);
		
	glBindVertexArray(0);
	//texture.unbind();

	shader.unbind();
	
}

void ParticleSystem::genParticleBuffers()
{
	// Vertex data
	GLfloat points[] = {
		0.0f,  0.0f // Top-left
	};

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //layout 0: position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	glGenBuffers(1, &particles_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, particles_buffer);
	glBufferData(GL_ARRAY_BUFFER, max_particles * sizeof(Particle), NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(1); // layout 1: offset
	glBindBuffer(GL_ARRAY_BUFFER, particles_buffer);
	glVertexAttribPointer(
		1, 
		3, // size : x + y + z 
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Particle), // stride
		(void*)0 // array buffer offset
	);

	glEnableVertexAttribArray(2); //layout 2: color
	glBindBuffer(GL_ARRAY_BUFFER, particles_buffer);
	glVertexAttribPointer(
		2,
		4, // size : rgba
		GL_FLOAT, // type
		GL_FALSE, 
		sizeof(Particle), 
		(GLvoid*)offsetof(Particle, color)
	);

	glEnableVertexAttribArray(3); //layout 3: size
	glBindBuffer(GL_ARRAY_BUFFER, particles_buffer);
	glVertexAttribPointer(
		3,
		1, // size 
		GL_FLOAT, 
		GL_FALSE, 
		sizeof(Particle), 
		(GLvoid*)offsetof(Particle, size)
	);

	glVertexAttribDivisor(0, 0); // particle center
	glVertexAttribDivisor(1, 1); // offset: one per quad
	glVertexAttribDivisor(2, 1); // color : one per quad -> 1
	glVertexAttribDivisor(3, 1); // size : one per quad -> 1
}

void ParticleSystem::updateParticle(Particle &particle, const Camera &camera)
{
	
	float deltaTime = 0.007f; // glfwGetTime() - currTime;//TODO set this to clock time instead of hardcoding
	currTime = (float)glfwGetTime();

	if (particle.isAlive()) {
		
		// Decrease life
		particle.life -= deltaTime; 

		if (particle.isAlive()) { //check to see if particle is still alive after shortening life
			emitter->simulate(particle, deltaTime);
			particle.camDist = pow(glm::length(particle.pos - camera.position), 2.0f);
		}
		else {
			particle.camDist = -1.0f;
			particle_count--;
		}
	}
}

int ParticleSystem::findLastUsed()
{
	for (int i = lastUsedIndex; i < max_particles; i++) {
		if (particles[i].life <= 0.0f) {
			lastUsedIndex = i;
			return i;
		}
	}

	for (int i = 0; i < lastUsedIndex; i++) {
		if (particles[i].life <= 0.0f) {
			lastUsedIndex = i;
			return i;
		}
	}

	return 0; //start over again from first particle
}

void ParticleSystem::spawn(int spawnIndex)
{
	
	emitter->spawn(particles[spawnIndex]);	
	
}

void ParticleSystem::sortParticles()
{
	std::sort(std::begin(particles), std::end(particles));
}

bool Particle::operator<(Particle & p)
{
	return this->camDist > p.camDist;
}

bool Particle::isAlive()
{
	return life > 0.0f;
}

Particle::Particle() : pos(0.0f), velocity(0.0f), color(1.0f), life(0.0f), size(1.0f)
{
}
