#include "sphere_mesh.h"

SphereMesh::SphereMesh(SphereMeshGeometry* geom) : Mesh(geom)
{
	//TODO set material funciton
	mtl.diffuse = glm::vec3(0.8f);
	mtl.ambient = glm::vec3(0.3f);
	mtl.specular = glm::vec3(0.0f);
	mtl.emission = glm::vec3(0.0f);
	mtl.shininess = 10.0f;

	createMesh();
}