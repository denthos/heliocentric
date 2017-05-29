#include "drawable_planet.h"

#include "sphere_model.h"
#include "drawable_slot.h"
#include <glm/gtx/transform.hpp>

std::unordered_map<PlanetType, DrawablePlanetData>& DrawablePlanet::getDataMap() {
	static std::unordered_map<PlanetType, DrawablePlanetData> drawable_data_map;
	if (drawable_data_map.empty()) {
		drawable_data_map.insert(std::make_pair(SUN, DrawablePlanetData{Texture::getInstance("Textures/sun.jpg")}));
		drawable_data_map.insert(std::make_pair(MERCURY, DrawablePlanetData{Texture::getInstance("Textures/mercury.jpg")}));
		drawable_data_map.insert(std::make_pair(VENUS, DrawablePlanetData{Texture::getInstance("Textures/venus.jpg")}));
		drawable_data_map.insert(std::make_pair(EARTH, DrawablePlanetData{Texture::getInstance("Textures/earth.jpg")}));
		drawable_data_map.insert(std::make_pair(MARS, DrawablePlanetData{Texture::getInstance("Textures/mars.jpg")}));
		drawable_data_map.insert(std::make_pair(JUPITER, DrawablePlanetData{Texture::getInstance("Textures/jupiter.jpg")}));
	}

	return drawable_data_map;
}

DrawablePlanet::DrawablePlanet(const Planet & planet) : Planet(planet) {
	this->toWorld = glm::translate(get_position()) * glm::scale(glm::vec3(get_radius()));
	model = new SphereModel(getDataMap().at(planet.get_type()).texture);

	/* Convert all the planet's slots to DrawableSlots */
	auto& slots = this->get_slots();
	for (auto slot_pair : slots) {
		slots[slot_pair.first] = new DrawableSlot(*slot_pair.second, this);
	}
}

DrawablePlanet::~DrawablePlanet() {

}

void DrawablePlanet::update() {
	this->toWorld = glm::translate(get_position()) * glm::scale(glm::vec3(get_radius()));

	for (auto slot_pair : this->get_slots_const()) {
		DrawableSlot* drawable_slot = static_cast<DrawableSlot*>(slot_pair.second);
		if (drawable_slot) {
			drawable_slot->update();
		}
	}
}
