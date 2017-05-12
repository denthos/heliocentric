#include "drawable_unit.h"
#include "drawable_planet.h"
#include "sphere_mesh.h"
#include "sphere_model.h"
#include <glm/gtx/transform.hpp>
DrawableUnit::DrawableUnit(const Unit & unit, Model* spaceship) : Unit(unit) {
	this->toWorld = glm::translate(unit.get_position()) * glm::scale(glm::vec3(0.3f));
	model = spaceship;
}

DrawableUnit::~DrawableUnit() {

}

void DrawableUnit::update() {
	glm::mat4 scale_mat = glm::scale(glm::vec3(0.3f));
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 xaxis = glm::cross(up, orientation);
	xaxis = glm::normalize(xaxis);
	glm::vec3 yaxis = glm::cross(orientation, xaxis);
	yaxis = glm::normalize(yaxis);
	
	rotation[0][0] = xaxis.x;
	rotation[0][1] = yaxis.x;
	rotation[0][2] = orientation.x;

	rotation[1][0] = xaxis.y;
	rotation[1][1] = yaxis.y;
	rotation[1][2] = orientation.y;

	rotation[2][0] = xaxis.z;
	rotation[2][1] = yaxis.z;
	rotation[2][2] = orientation.z;
	

	this->toWorld = glm::translate(get_position()) * rotation * scale_mat;
}