#include "rocket.h"

Rocket::Rocket(Model & model)
{
	rocketModel = model;
}

void Rocket::TurnTowards(glm::vec3 point)
{
	
	glm::mat4 lookAt = glm::lookAt(pos, point, up);
	glm::quat toRotQuat = glm::quat_cast( glm::transpose(lookAt)); //the final rotation quaternion

	glm::quat rotQuat = glm::slerp( , toRotQuat);

}
