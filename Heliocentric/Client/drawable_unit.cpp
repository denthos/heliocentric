#include "drawable_unit.h"
#include "drawable_planet.h"
#include "sphere_mesh.h"
#include "sphere_model.h"

DrawableUnit::DrawableUnit(const Unit & unit) : Unit(unit) {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(20.0f)), unit.get_position());
	model = new SphereModel(DrawablePlanet::getDataMap().at(PlanetType::EARTH).texture);
}

DrawableUnit::~DrawableUnit() {

}

void DrawableUnit::update() {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(20.0f)), get_position());
}
