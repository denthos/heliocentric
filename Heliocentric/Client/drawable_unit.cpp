#include "drawable_unit.h"
#include "drawable_planet.h"
#include "sphere_mesh.h"
#include "sphere_model.h"
#include "gui.h"
#include "particle_system.h"
#include <glm/gtx/transform.hpp>

#define ROCKET_MODEL "Models/Federation Interceptor HN48/Federation Interceptor HN48 flying.obj"
#define BEAR_MODEL "Models/Kameri explorer/Kameri explorer flying.obj"

const std::unordered_map<UnitType::TypeIdentifier, DrawableUnitData>& DrawableUnit::getDataMap() {
	static std::unordered_map<UnitType::TypeIdentifier, DrawableUnitData> dataMap {
		{UnitType::BASIC_UNIT, DrawableUnitData {Model::getInstance(ROCKET_MODEL), 0.1f}},
		{UnitType::HEAVY_UNIT, DrawableUnitData {Model::getInstance(BEAR_MODEL), 0.8f}}
	};

    return dataMap;
}


DrawableUnit::DrawableUnit(const Unit & unit, Shader * shader, ParticleSystem* laser) : 
	Unit(unit), rotation_matrix(glm::mat4(1.0f)), old_orientation(glm::vec3(0.0f, 0.0f, 1.0f)), laser(laser) {
	this->data = getDataMap().at(getType()->getIdentifier());
	this->shader = shader;

	this->toWorld = glm::translate(get_position()) * glm::scale(glm::vec3(data.scalingFactor));
	this->model = data.model;
	this->glow = false;

    BoundingBox bbox = getBoundingBox();
    glm::vec3 b_max = bbox.max;
    glm::vec3 b_min = bbox.min;
    float z_center = (b_max.z + b_min.z) / 2.0f;
    this->laser_offset = glm::vec3(0.0f, 60.0f, b_max.z - z_center);
}

DrawableUnit::~DrawableUnit() {

}

void DrawableUnit::update() {
	this->updateRotationMatrix();
	this->toWorld = glm::translate(get_position()) * getRotationMatrix() * glm::scale(glm::vec3(data.scalingFactor));
}

void DrawableUnit::draw(const Camera & camera) const {
    PlayerColor::Color player_color = this->get_player()->getColor();
    shader->bind();
    GLuint shaderID = shader->getPid();

	//highlight selected unit
	glUniform1i(glGetUniformLocation(shader->getPid(), "glow"), glow);
	shader->bind();


    //color cities based on player color
    glm::vec3 rgbVec = PlayerColor::colorToRGBVec(player_color);
    glUniform3f(glGetUniformLocation(shaderID, "m_color"), rgbVec.x, rgbVec.y, rgbVec.z);
    Drawable::draw(camera);

	glUniform1i(glGetUniformLocation(shader->getPid(), "glow"), 0);
	shader->unbind();

	if (this->client_isattacking) {
		laser->Update(camera);
		laser->draw(camera, glm::translate(toWorld, this->laser_offset));
	}
}


void DrawableUnit::select(GUI* gui, Client* client) {
	gui->showUnitUI(this);
	glow = true;
}

void DrawableUnit::unselect(GUI* gui, Client* client) {
	gui->hideUnitUI();
	glow = false;
}

glm::mat4 DrawableUnit::getRotationMatrix() const {
	return this->rotation_matrix;
}

void DrawableUnit::updateRotationMatrix() {
	if (glm::distance(old_orientation, orientation) <= 0.005) {
		return;
	}

	float cosine = glm::clamp(glm::dot(old_orientation, orientation), -1.0f, 1.0f);

	float denom = glm::length(old_orientation) * glm::length(orientation);

	/* If we are about to do bad division, just bail out*/
	if (denom == 0) {
		return;
	}

	float cosine_ratio = cosine / denom;

	/* If we are about to do acos() on something outside its domain, bail out*/
	if (cosine_ratio < -1 || cosine_ratio > 1) {
		return;
	}

	float rotAngle = glm::acos(cosine / denom);
	glm::vec3 cross = glm::cross(old_orientation, orientation);

	if (cross.y < 0) {
		rotAngle = -rotAngle;
	}

	this->rotation_matrix = glm::rotate(this->rotation_matrix, rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	this->old_orientation = orientation;
}
