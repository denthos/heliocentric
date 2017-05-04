#include "drawable_planet.h"

#include "sphere_model.h"

std::unordered_map<PlanetType, DrawableData>& DrawablePlanet::getDataMap() {
	static std::unordered_map<PlanetType, DrawableData> drawable_data_map;
	if (drawable_data_map.empty()) {
		drawable_data_map[SUN] = DrawableData(Texture("Textures/sun.jpg"));
		drawable_data_map[MERCURY] = DrawableData(Texture("Textures/mercury.jpg"));
		drawable_data_map[VENUS] = DrawableData(Texture("Textures/venus.jpg"));
		drawable_data_map[EARTH] = DrawableData(Texture("Textures/earth.jpg"));
		drawable_data_map[MARS] = DrawableData(Texture("Textures/mars.jpg"));
		drawable_data_map[JUPITER] = DrawableData(Texture("Textures/jupiter.jpg"));
	}

	return drawable_data_map;
}

DrawablePlanet::DrawablePlanet(const Planet & planet) : Planet(planet) {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(get_radius())), planet.get_position());
	model = new SphereModel(getDataMap().at(planet.get_type()).texture);
}

DrawablePlanet::~DrawablePlanet() {

}

void DrawablePlanet::update() {
	this->toWorld = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(get_radius())), get_position());
}