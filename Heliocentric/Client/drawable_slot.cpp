#include "drawable_slot.h"
#include "sphere_model.h"
#include "planet.h"

DrawableSlot::DrawableSlot(const Slot& slot, Planet* planet) : Slot(slot) {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(slot.planet->get_radius())), slot.planet->get_position() + slot.get_position());
	//this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), slot.planet->get_radius()), slot.get_position());
	model = new SphereModel(Texture::getTexture("Textures/sun.jpg"));
	this->planet = planet;
}

void DrawableSlot::update() {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(planet->get_radius())), planet->get_position() + get_position());
}