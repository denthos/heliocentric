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
	
	

	this->toWorld = glm::translate(get_position()) * get_rotation() * scale_mat;
}