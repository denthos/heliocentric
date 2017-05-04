#include "drawable_unit.h"
#include "sphere_mesh.h"

DrawableUnit::DrawableUnit(const Unit & unit) : Unit(unit) {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(20.0f)), unit.get_position());
	mesh = new SphereMesh();
	texture = DrawableData(Texture("Textures/earth.jpg"));
	mesh->setTexture(texture.texture);
}

DrawableUnit::~DrawableUnit() {

}

void DrawableUnit::update() {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(20.0f)), get_position());
}