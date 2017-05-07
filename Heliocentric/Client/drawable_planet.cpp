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
	this->toWorld = glm::scale(glm::translate(glm::mat4(1.0f), planet.get_position()), glm::vec3(get_radius()));
	model = new SphereModel(getDataMap().at(planet.get_type()).texture);
}

DrawablePlanet::~DrawablePlanet() {

}

void DrawablePlanet::update() {
	this->toWorld = glm::scale(glm::translate(glm::mat4(1.0f), get_position()), glm::vec3(get_radius()));
}