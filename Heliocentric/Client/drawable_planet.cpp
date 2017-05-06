#include "drawable_planet.h"

#include "sphere_model.h"

std::unordered_map<PlanetType, DrawableData>& DrawablePlanet::getDataMap() {
	static std::unordered_map<PlanetType, DrawableData> drawable_data_map;
	if (drawable_data_map.empty()) {
		drawable_data_map.insert(std::make_pair(SUN, DrawableData(Texture::getTexture("Textures/sun.jpg"))));
		drawable_data_map.insert(std::make_pair(MERCURY, DrawableData(Texture::getTexture("Textures/mercury.jpg"))));
		drawable_data_map.insert(std::make_pair(VENUS, DrawableData(Texture::getTexture("Textures/venus.jpg"))));
		drawable_data_map.insert(std::make_pair(EARTH, DrawableData(Texture::getTexture("Textures/earth.jpg"))));
		drawable_data_map.insert(std::make_pair(MARS, DrawableData(Texture::getTexture("Textures/mars.jpg"))));
		drawable_data_map.insert(std::make_pair(JUPITER, DrawableData(Texture::getTexture("Textures/jupiter.jpg"))));
	}

	return drawable_data_map;
}

DrawablePlanet::DrawablePlanet(const Planet & planet) : Planet(planet) {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(get_radius())), planet.get_position());
	model = new SphereModel(getDataMap().at(planet.get_type()).texture);

	/* Now load all the slots into drawble slots*/
	for (auto slot_pair : this->get_slots()) {
		this->drawable_slots.insert(
			std::make_pair(slot_pair.first, DrawableSlot(*(slot_pair.second), this))
		);
	}
}


void DrawablePlanet::draw(const Shader & shader, const Camera & camera) const {
	/* First, draw the planet model */
	this->model->draw(shader, camera, this->toWorld);

	/* Now let's draw all the slots */
	for (auto& drawable_slot_pair : this->drawable_slots) {
		drawable_slot_pair.second.draw(shader, camera);
	}
}

DrawablePlanet::~DrawablePlanet() {

}

void DrawablePlanet::update() {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(get_radius())), get_position());

	/* Now let's update all the slots */
	for (auto& drawable_slot_pair : this->drawable_slots) {
		drawable_slot_pair.second.update();
	}
}