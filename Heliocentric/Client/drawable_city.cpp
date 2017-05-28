#include "drawable_city.h"
#include "drawable_planet.h"
#include "sphere_model.h"
#include "drawable_slot.h"
#include "gui.h"
#include "client.h"

#include <glm/gtx/transform.hpp>

DrawableCity::DrawableCity(const City& city) : City(city) {
	this->model = Model::getInstance("Models/city.obj");
	this->drawable_slot = static_cast<DrawableSlot*>(this->get_slot());

	this->toWorld = this->drawable_slot->getToWorld();
}

void DrawableCity::update() {
	this->toWorld = this->drawable_slot->getToWorld();
}


void DrawableCity::select(GUI* gui, Client* client) {
	gui->displayCityUI(this, std::bind(&Client::createUnitFromCity, client, this, std::placeholders::_1));
}

void DrawableCity::unselect(GUI* gui, Client* client) {
	gui->hideCityUI();
}
