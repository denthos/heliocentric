#pragma once
#include "camera.h"
#include "sphere_mesh.h"
#include "orbit.h"
class PlanetModel {
public:
	//sphere mesh
	SphereMesh planet_mesh;
	Texture planet_texture;
	Orbit orbit;
	glm::mat4 world_mat;
	float radius;
	
	PlanetModel() {}
	PlanetModel(Texture texture, float radius, Orbit orbit);
	~PlanetModel();


	void Draw(Shader &shader, const Camera &);
	void Update(glm::mat4 &parent);

};