#include "drawable_city.h"
#include "drawable_planet.h"
#include "sphere_model.h"

#include <glm/gtx/transform.hpp>

DrawableCity::DrawableCity(const City& city) : City(city) {
	this->model = new Model("Models/city.obj");
	this->toWorld = glm::translate(get_slot()->get_absolute_position()) *  get_slot()->get_spherical_position().getRotationMatrix() * glm::scale(glm::vec3(get_slot()->getPlanet()->get_radius() / 10.0f));
}

void DrawableCity::update() {
	this->toWorld = glm::translate(get_slot()->get_absolute_position()) *  get_slot()->get_spherical_position().getRotationMatrix() * glm::scale(glm::vec3(get_slot()->getPlanet()->get_radius() / 10.0f));
}
