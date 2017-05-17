#include "drawable_unit.h"
#include "drawable_planet.h"
#include "sphere_mesh.h"
#include "sphere_model.h"
#include <glm/gtx/transform.hpp>
DrawableUnit::DrawableUnit(const Unit & unit, Model* spaceship, Shader* unitShader, ParticleSystem* laser, ParticleSystem* explosion) : Unit(unit) {
	this->toWorld = glm::translate(unit.get_position()) * glm::scale(glm::vec3(0.3f));
	model = spaceship;
	this->laser = laser;
	this->explosion = explosion;

	//calculate the shooting offset
	BoundingBox bbox = getBoundingBox();
	glm::vec3 b_max = bbox.max;
	glm::vec3 b_min = bbox.min;
	float z_center = (b_max.z + b_min.z) / 2.0f;
	shooting_offset = glm::vec3(0.0f, 60.0f, b_max.z - z_center);
	explosion_counter = 0;
	this->unitShader = unitShader;
	

}

DrawableUnit::~DrawableUnit() {

}

void DrawableUnit::update() {
	glm::mat4 scale_mat = glm::scale(glm::vec3(0.3f));
	this->toWorld = glm::translate(get_position()) * get_rotation() * scale_mat;
	if (explosion_counter != 100 && is_exploding == true) {
		explosion_counter += 1;
	}
	else if (is_exploding == true) {
		explosion_counter = 0;
		is_exploding = false;
	}
}

void DrawableUnit::draw(const Shader & shader, const Camera & camera) const {
	if (is_exploding) {
		unitShader->bind();
		glUniform1i(glGetUniformLocation(unitShader->getPid(), "explode_on"), true);

		explosion->Update(camera);
		explosion->draw(camera, glm::scale(toWorld, glm::vec3(20.0f))); //needs to be shifted a bit, bigger pixels?
	}

	model->draw(*unitShader, camera, toWorld);

	if(this->shoot_laser){
		laser->Update(camera); //probably should go in update function but i need access to the camera somehow...
		laser->draw(camera, glm::translate(toWorld, shooting_offset));
	}
	
}

void DrawableUnit::bind_shader(Shader* shader) {
	this->unitShader = shader;
}


bool DrawableUnit::do_animation(const Shader & shader, const Camera & camera) const {
	draw(shader, camera);
	return true;
}