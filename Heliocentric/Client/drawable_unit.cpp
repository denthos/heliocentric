#include "drawable_unit.h"

#include "sphere_mesh.h"

DrawableUnit::DrawableUnit(const Unit & unit, Texture texture) : Unit(unit) {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(20.0f)), unit.get_position());
	mesh = new SphereMesh();
	mesh->setTexture(texture);
}

DrawableUnit::~DrawableUnit() {

}

void DrawableUnit::update() {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(20.0f)), get_position());
}