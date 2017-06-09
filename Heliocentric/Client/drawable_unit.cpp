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


DrawableUnit::DrawableUnit(const Unit & unit, Shader * shader, ParticleSystem* laser, ParticleSystem* explosion, ThreeDSoundSystem* sound_system, PlayerIcon* icon) : 
	Unit(unit), rotation_matrix(glm::mat4(1.0f)), old_orientation(glm::vec3(0.0f, 0.0f, 1.0f)), laser(laser), explosion(explosion), icon(icon) {
    this->data = getDataMap().at(getType()->getIdentifier());
    this->shader = shader;

    this->toWorld = glm::translate(get_position()) * glm::scale(glm::vec3(data.scalingFactor));
    this->model = data.model;
    this->laser = laser;
    this->glow = false;
	this->is_exploding = false;
	this->explosion_counter = 0.0f;



    BoundingBox bbox = getBoundingBox();
    glm::vec3 b_max = bbox.max;
    glm::vec3 b_min = bbox.min;
    float z_center = (b_max.z + b_min.z) / 2.0f;
    this->laser_offset = glm::vec3(0.0f, 60.0f, b_max.z - z_center);
	this->shoot_sound = new Audio3DSound(sound_system, "Audio/laser1.wav");
}

DrawableUnit::~DrawableUnit() {

}

void DrawableUnit::update() {
	this->updateRotationMatrix();
	this->toWorld = glm::translate(get_position()) * getRotationMatrix() * glm::scale(glm::vec3(data.scalingFactor));
	shoot_sound->update(this->get_position());
	if (explosion_counter < pi/2.0f && is_exploding == true) {
		LOG_DEBUG("Explosion counter is ", explosion_counter);
		explosion_counter += pi/50.0f;
	}
	else if (is_exploding == true) {
		LOG_DEBUG("Explosion counter stopped.");
		explosion_counter = 0.0f;
		is_exploding = false;
	}
}

void DrawableUnit::draw(const Camera & camera) const {

	icon->setColor(glm::vec4(PlayerColor::colorToRGBVec(this->get_player()->getColor()), 1.0f));
	icon->setSize(0.01);
	icon->update(toWorld[3]);
	icon->draw(camera);

    PlayerColor::Color player_color = this->get_player()->getColor();
    shader->bind();
    GLuint shaderID = shader->getPid();

	//highlight selected unit
	glUniform1i(glGetUniformLocation(shader->getPid(), "glow"), glow);

	if (is_exploding) {
		glUniform1i(glGetUniformLocation(shader->getPid(), "explode_on"), is_exploding);
		glUniform1f(glGetUniformLocation(shader->getPid(), "timer"), explosion_counter);
	}
    //color cities based on player color
    glm::vec3 rgbVec = PlayerColor::colorToRGBVec(player_color);
    glUniform3f(glGetUniformLocation(shaderID, "m_color"), rgbVec.x, rgbVec.y, rgbVec.z);
    Drawable::draw(camera);
	shader->bind();
	glUniform1i(glGetUniformLocation(shader->getPid(), "glow"), 0);
	glUniform1i(glGetUniformLocation(shader->getPid(), "explode_on"), false);
	shader->unbind();

	// unit explosion when dead
	if (is_exploding) {
		LOG_DEBUG("Is exploding");
		
		explosion->Update(camera);
		explosion->draw(camera, glm::scale(toWorld, glm::vec3(20.0f))); //needs to be shifted a bit, bigger pixels?
	
	}

	if (this->client_isAttacking()) {
		shoot_sound->play(get_position());
		laser->Update(camera);
		//TODO uncomment or fix with master always shooting laser;
		laser->draw(camera, glm::translate(toWorld, this->laser_offset));
	}
}

bool DrawableUnit::do_animation(const Camera & camera) const {
	if (is_exploding)
		draw(camera);
	return is_exploding;
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
	float cosine_ratio = cosine / denom;

	float rotAngle = glm::acos(cosine_ratio);

	/* 
	It is entirely possible that something went wrong with our angle. If something
	did go wrong, let's just bail out
	*/
	if (std::isnan(rotAngle)) {
		return;
	}

	glm::vec3 cross = glm::cross(old_orientation, orientation);

	if (cross.y < 0) {
		rotAngle = -rotAngle;
	}

	this->rotation_matrix = glm::rotate(this->rotation_matrix, rotAngle, glm::vec3(0.0f, 1.0f, 0.0f));
	this->old_orientation = orientation;
}
