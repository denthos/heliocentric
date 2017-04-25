#include "planet.h"


PlanetModel::PlanetModel(Texture texture, float radius, Orbit orbit) {
	world_mat = glm::mat4(1.0f);
	this->orbit = orbit;
	planet_texture = texture;

	planet_mesh = SphereMesh();
	planet_mesh.scale_mat = glm::scale(glm::mat4(1.0f), glm::vec3(radius));
	planet_mesh.trans_mat = orbit.getTranslateMat();
	planet_mesh.setTexture(texture);

	
}

PlanetModel::~PlanetModel()
{
}

void PlanetModel::Draw(Shader &shader, const Camera & camera)
{
	planet_mesh.Draw(shader, camera);
}

void PlanetModel::Update(glm::mat4 & parent)
{
	planet_mesh.rot_mat = planet_mesh.rot_mat * orbit.getOrbitMat();
	planet_mesh.Update(parent * world_mat  );
}
