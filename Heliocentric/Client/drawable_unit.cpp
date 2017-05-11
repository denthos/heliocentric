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
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)), get_position());
}