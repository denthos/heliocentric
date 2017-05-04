#include "rocket.h"

Rocket::Rocket(Model *model)
{
	this->model = model;
}

Rocket::~Rocket()
{
}

void Rocket::TurnTowards(glm::vec3 point)
{
	float time = 1.0f; //TODO
	glm::mat4 lookAt = glm::lookAt(position, point, up);
	glm::quat toRotQuat = glm::quat_cast(glm::transpose(lookAt)); //the final rotation quaternion

	//for smooth rotation
	glm::quat rotQuat = glm::slerp(glm::quat_cast(rot_mat), toRotQuat, time);

	//convert back to matrix, save and apply
	rot_mat = glm::mat4_cast(rotQuat);

}

void Rocket::draw(const Shader & shader, const Camera & camera, const glm::mat4 & toWorld)
{
	model->draw(shader, camera, toWorld);
}

void Rocket::update(glm::vec3 point)
{
	if (turn) {
		TurnTowards();


	
	}

}
