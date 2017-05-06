#include "rocket.h"
#include <iostream>

Rocket::Rocket(Model *model)
{
	this->model = model;
}

Rocket::~Rocket()
{
}

void Rocket::TurnTowards(glm::vec3 point)
{
	//NEED: center and front of model to calculate orientation
	if(start == 0.0f){
		start = std::clock();
	}
	float currTime = std::clock();
	float deltaTime = glm::clamp((currTime - start) / (float)CLOCKS_PER_SEC, 0.0f, 1.0f);

	//NEED TO CHANGE THESE VECTORS TO ORIGIN-DESTINATION AND ORIENTATION VECTOR
	glm::vec3 orientation(0.0f, 0.0f, 1.0f); //TODO
	glm::vec3 toPoint = point - position;

	toPoint = (toPoint == glm::vec3(0.0f)) ? toPoint : glm::normalize(toPoint);



	float cosine = glm::dot(orientation, toPoint);
	cosine = glm::clamp(cosine, -1.0f, 1.0f);

	glm::vec3 axis = glm::cross(orientation, toPoint);

	axis =  glm::normalize(axis);


	float rotAngle = glm::degrees(glm::acos(cosine));
	glm::mat4 lookAt = glm::rotate(glm::mat4(1.0f), rotAngle, axis);
	glm::quat toRotQuat = glm::quat_cast((lookAt)); //the final rotation quaternion

	
	
	//for smooth rotation
	glm::quat rotQuat = glm::slerp(glm::quat_cast(rot_mat), toRotQuat, deltaTime);

	//convert back to matrix, save and apply
	rot_mat = glm::mat4_cast(rotQuat);

	if (deltaTime == 1.0f) {
		turn = false;
		start = 0.0f;
	}



}

void Rocket::draw(const Shader & shader, const Camera & camera, const glm::mat4 & toWorld)
{
	model->draw(shader, camera, toWorld * rot_mat);
}

void Rocket::update(glm::vec3 point)
{
	if (turn) {

		
		TurnTowards(point);


	
	}

}
