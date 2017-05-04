#pragma once
#include "model.h"
#include "drawable_unit.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\quaternion.hpp>

class Rocket{ //TODO extend drawable unit
public:
	Rocket(Model *model);
	~Rocket();

	//animation sequences
	void TurnTowards(glm::vec3 point);
	void ShootLaser();

	void draw(const Shader & shader, const Camera & camera, const glm::mat4 & toWorld);
	void update(glm::vec3 point);

private:
	Model *model; //TODO remove
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); //up direction is always positive y axis
	glm::mat4 rot_mat = glm::mat4(1.0f);
	glm::vec3 position = glm::vec3(0.0f); //TODO remove

	bool turn = false;
	
	

};