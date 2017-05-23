#include "drawable_slot.h"
#include "drawable_planet.h"
#include "drawable_city.h"
#include "sphere_model.h"

#include "client.h"
#include "gui.h"

#include <glm/gtx/transform.hpp>

DrawableSlot::DrawableSlot(const Slot& slot, DrawablePlanet* planet) : Slot(slot) {
	this->planet = planet;
	this->model = Model::getInstance("Models/donut.obj");
	this->toWorld = glm::translate(get_position()) *  get_spherical_position().getRotationMatrix() * glm::scale(glm::vec3(planet->get_radius() / 10.0f));
}

void DrawableSlot::update() {
	this->toWorld = glm::translate(get_position()) *  get_spherical_position().getRotationMatrix() * glm::scale(glm::vec3(planet->get_radius() / 10.0f));

	if (this->hasCity()) {
		DrawableCity* drawable_city = dynamic_cast<DrawableCity*>(this->city);
		if (drawable_city) {
			drawable_city->update();
		}
	}
}

void DrawableSlot::select(GUI* gui, Client* client) {
	gui->displaySlotUI(this, std::bind(&Client::createCityForSlot, client, this, std::placeholders::_1));
}


void DrawableSlot::draw(const Shader & shader, const Camera & camera) const {
	if (!hasCity()) {
		Drawable::draw(shader, camera);
	}
}

void DrawableSlot::unselect(GUI* gui, Client* client) {
	gui->hideSlotUI();
}

Selectable* DrawableSlot::getSelection() {
	if (hasCity()) {
		DrawableCity* selectable_city = dynamic_cast<DrawableCity*>(getCity());
		return selectable_city;
	}

	return this;
}
