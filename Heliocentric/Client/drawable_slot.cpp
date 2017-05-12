#include "drawable_slot.h"
#include "drawable_planet.h"
#include "drawable_city.h"
#include "sphere_model.h"

DrawableSlot::DrawableSlot(const Slot& slot, DrawablePlanet* planet) : Slot(slot) {
	this->planet = planet;
	this->model = new SphereModel(Texture::getInstance("Textures/sun.jpg"));
	this->toWorld = glm::translate(glm::mat4(1.0f), get_absolute_position());
}

void DrawableSlot::update() {
	if (this->hasCity()) {
		DrawableCity* drawable_city = dynamic_cast<DrawableCity*>(this->city);
		if (drawable_city) {
			drawable_city->update();
		}
	}
	else {
		this->toWorld = glm::translate(glm::mat4(1.0f), get_absolute_position());
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
