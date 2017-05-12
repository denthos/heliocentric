#include "drawable_planet.h"

#include "sphere_model.h"
#include "drawable_slot.h"

std::unordered_map<PlanetType, DrawableData>& DrawablePlanet::getDataMap() {
	static std::unordered_map<PlanetType, DrawableData> drawable_data_map;
	if (drawable_data_map.empty()) {
		drawable_data_map.insert(std::make_pair(SUN, DrawableData(Texture::getInstance("Textures/sun.jpg"))));
		drawable_data_map.insert(std::make_pair(MERCURY, DrawableData(Texture::getInstance("Textures/mercury.jpg"))));
		drawable_data_map.insert(std::make_pair(VENUS, DrawableData(Texture::getInstance("Textures/venus.jpg"))));
		drawable_data_map.insert(std::make_pair(EARTH, DrawableData(Texture::getInstance("Textures/earth.jpg"))));
		drawable_data_map.insert(std::make_pair(MARS, DrawableData(Texture::getInstance("Textures/mars.jpg"))));
		drawable_data_map.insert(std::make_pair(JUPITER, DrawableData(Texture::getInstance("Textures/jupiter.jpg"))));
	}

	return drawable_data_map;
}

DrawablePlanet::DrawablePlanet(const Planet & planet) : Planet(planet) {
	this->toWorld = glm::scale(glm::translate(glm::mat4(1.0f), planet.get_position()), glm::vec3(get_radius()));
	model = new SphereModel(getDataMap().at(planet.get_type()).texture);

	/* Convert all the planet's slots to DrawableSlots */
	auto& slots = this->get_slots();
	for (auto slot_pair : slots) {
		slots[slot_pair.first] = new DrawableSlot(*slot_pair.second, this);
	}
}

void DrawablePlanet::draw(const Shader& shader, const Camera& camera) const {
	Drawable::draw(shader, camera);

	for (auto slot_pair : this->get_slots_const()) {
		DrawableSlot* drawable_slot = static_cast<DrawableSlot*>(slot_pair.second);
		if (drawable_slot) {
			drawable_slot->draw(shader, camera);
		}
	}
}

DrawablePlanet::~DrawablePlanet() {

}

void DrawablePlanet::update() {
	this->toWorld = glm::scale(glm::translate(glm::mat4(1.0f), get_position()), glm::vec3(get_radius()));

	for (auto slot_pair : this->get_slots_const()) {
		DrawableSlot* drawable_slot = static_cast<DrawableSlot*>(slot_pair.second);
		if (drawable_slot) {
			drawable_slot->update();
		}
	}
}
