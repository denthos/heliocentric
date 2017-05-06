#include "drawable_city.h"
#include "sphere_model.h"
#include "planet.h"

DrawableCity::DrawableCity(const City& city, Slot* slot) : City(city) {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(slot->planet->get_radius())), slot->planet->get_position() + city.get_position());
	model = new SphereModel(Texture::getTexture("Textures/mars.jpg"));

	this->slot = slot;
	this->slot->city = this;
}

void DrawableCity::update() {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(slot->planet->get_radius())), slot->planet->get_position() + get_position());
}