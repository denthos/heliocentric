#include "drawable_slot.h"
#include "drawable_planet.h"
#include "drawable_city.h"
#include "sphere_model.h"
#include "logging.h"

#include <glm/gtx/transform.hpp>

DrawableSlot::DrawableSlot(const Slot& slot, DrawablePlanet* planet) : Slot(slot) {
	this->planet = planet;
	this->model = Model::getInstance("Models/donut.obj");
	this->toWorld = glm::translate(get_position()) *  get_spherical_position().getRotationMatrix() * glm::scale(glm::vec3(planet->get_radius() / 10.0f));

	LOG_DEBUG("SLOT:", getBoundingBox().max.x, ",", getBoundingBox().max.y, ",", getBoundingBox().max.z);
	LOG_DEBUG("PLANET:", planet->getBoundingBox().max.x, ",", planet->getBoundingBox().max.y, ",", planet->getBoundingBox().max.z);
}

void DrawableSlot::update() {
	if (this->hasCity()) {
		DrawableCity* drawable_city = dynamic_cast<DrawableCity*>(this->city);
		if (drawable_city) {
			drawable_city->update();
		}
	}
	else {
		this->toWorld = glm::translate(get_position()) *  get_spherical_position().getRotationMatrix() * glm::scale(glm::vec3(planet->get_radius() / 10.0f));
	}
}

void DrawableSlot::draw(const Shader& shader, const Camera& camera) const {
	if (this->hasCity()) {
		DrawableCity* drawable_city = dynamic_cast<DrawableCity*>(this->city);
		if (drawable_city) {
			drawable_city->draw(shader, camera);
		}
	}
	else {
		Drawable::draw(shader, camera);
	}
}