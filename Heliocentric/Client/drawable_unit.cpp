#include "drawable_unit.h"
#include "drawable_planet.h"
#include "sphere_mesh.h"
#include "sphere_model.h"
#include <glm/gtx/transform.hpp>
DrawableUnit::DrawableUnit(const Unit & unit, Model* spaceship, ParticleSystem* laser) : Unit(unit) {
	this->toWorld = glm::translate(unit.get_position()) * glm::scale(glm::vec3(0.3f));
	model = spaceship;
	this->laser = laser;
}

DrawableUnit::~DrawableUnit() {

}

void DrawableUnit::update() {
	glm::mat4 scale_mat = glm::scale(glm::vec3(0.3f));
	
	

	this->toWorld = glm::translate(get_position()) * get_rotation() * scale_mat;
}

void DrawableUnit::draw(const Shader & shader, const Camera & camera) const {
	model->draw(shader, camera, toWorld);
	laser->Update(camera);
	laser->draw(camera, toWorld);
}