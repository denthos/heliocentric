#include "drawable_planet.h"

#include "sphere_mesh.h"

DrawablePlanet::DrawablePlanet(const Planet & planet, Texture texture) : Planet(planet) {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(get_radius())), planet.get_position());
	mesh = new SphereMesh();
	mesh->setTexture(texture);
}

DrawablePlanet::~DrawablePlanet() {

}

void DrawablePlanet::update() {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(get_radius())), get_position());
}