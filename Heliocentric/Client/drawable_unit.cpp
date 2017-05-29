#include "drawable_unit.h"
#include "drawable_planet.h"
#include "sphere_mesh.h"
#include "sphere_model.h"
#include "gui.h"
#include <glm/gtx/transform.hpp>

#define ROCKET_MODEL "Models/Federation Interceptor HN48/Federation Interceptor HN48 flying.obj"
#define BEAR_MODEL "Models/bear.obj"

const std::unordered_map<UnitType::TypeIdentifier, DrawableUnitData>& DrawableUnit::getDataMap() {
	static std::unordered_map<UnitType::TypeIdentifier, DrawableUnitData> dataMap {
		{UnitType::BASIC_UNIT, DrawableUnitData {Model::getInstance(ROCKET_MODEL), 0.1f}},
		{UnitType::HEAVY_UNIT, DrawableUnitData {Model::getInstance(BEAR_MODEL), 1.0f}}
	};

	return dataMap;
}

DrawableUnit::DrawableUnit(const Unit & unit) : Unit(unit) {
	this->data = getDataMap().at(getType()->getIdentifier());

	this->toWorld = glm::translate(get_position()) * glm::scale(glm::vec3(data.scalingFactor));
	this->model = data.model;
}

DrawableUnit::~DrawableUnit() {

}

void DrawableUnit::update() {
	this->toWorld = glm::translate(get_position()) * glm::scale(glm::vec3(data.scalingFactor));
}

void DrawableUnit::select(GUI* gui, Client* client) {
	gui->showUnitUI(this);
}

void DrawableUnit::unselect(GUI* gui, Client* client) {
	gui->hideUnitUI();
}