#include "drawable_unit.h"
#include "drawable_planet.h"
#include "sphere_mesh.h"
#include "sphere_model.h"
#include <glm/gtx/transform.hpp>
#define ROCKET_MODEL "Models/Federation Interceptor HN48/Federation Interceptor HN48 flying.obj"

DrawableUnit::DrawableUnit(const Unit & unit, Model* spaceship) : Unit(unit) {
	this->toWorld = glm::translate(get_position()) * glm::scale(glm::vec3(0.1f));
	model = spaceship;
}

DrawableUnit::~DrawableUnit() {

}

void DrawableUnit::update() {
	this->toWorld = glm::translate(get_position()) * glm::scale(glm::vec3(0.1f));
}