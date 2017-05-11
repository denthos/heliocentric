#include "drawable_unit.h"
#include "drawable_planet.h"
#include "sphere_mesh.h"
#include "sphere_model.h"

DrawableUnit::DrawableUnit(const Unit & unit, Model* spaceship) : Unit(unit) {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)), unit.get_position());
	model = spaceship;
}

DrawableUnit::~DrawableUnit() {

}

void DrawableUnit::update() {
	glm::mat4 scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
	glm::vec3 up(0.0f, 1.0f, 0.0f);
	glm::vec3 xaxis = glm::cross(up, orientation);
	xaxis = glm::normalize(xaxis);
	glm::vec3 yaxis = glm::cross(orientation, xaxis);
	yaxis = glm::normalize(yaxis);

	rotation[0][0] = xaxis.x;
	rotation[1][0] = yaxis.x;
	rotation[2][0] = orientation.x;

	rotation[0][1] = xaxis.y;
	rotation[1][1] = yaxis.y;
	rotation[2][1] = orientation.y;

	rotation[0][2] = xaxis.z;
	rotation[1][2] = yaxis.z;
	rotation[2][2] = orientation.z;

	this->toWorld = glm::translate(scale_mat * rotation, get_position());
}