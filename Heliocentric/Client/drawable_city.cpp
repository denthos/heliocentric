#include "drawable_city.h"
#include "drawable_planet.h"
#include "sphere_model.h"

DrawableCity::DrawableCity(const City& city) : City(city) {
	this->model = new SphereModel(Texture::getInstance("Textures/jupiter.jpg"));
	this->toWorld = glm::translate(glm::mat4(1.0f), get_position());
}

void DrawableCity::update() {
	this->toWorld = glm::translate(glm::mat4(1.0f), get_position());
}