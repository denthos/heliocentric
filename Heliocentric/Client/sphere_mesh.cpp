#include "sphere_mesh.h"

bool SphereMesh::init = false;

SphereMesh::SphereMesh()
{
	radius = 1.0f;
	rings = 24;
	sectors = 48;

	//TODO set material funciton
	mtl.diffuse = glm::vec3(0.8f);
	mtl.ambient = glm::vec3(0.3f);
	mtl.specular = glm::vec3(0.0f);
	mtl.emission = glm::vec3(0.0f);
	mtl.shininess = 10.0f;

	genMesh();

	createMesh();
}

SphereMesh::SphereMesh(float radius, int rings, int sectors)
{
	this->radius = radius;
	this->rings = rings;
	this->sectors = sectors;

	//TODO set material funciton
	mtl.diffuse = glm::vec3(0.0f);
	mtl.ambient = glm::vec3(0.0f);
	mtl.specular = glm::vec3(0.0f);
	mtl.emission = glm::vec3(0.0f);
	mtl.shininess = 10.0f;

	createMesh();
}

SphereMesh::~SphereMesh()
{
}

void SphereMesh::genMesh()
{
	//generate data for sphere mesh data buffers
	float const R = 1.0f / (float)(rings - 1); //what fraction of the sphere each ring division is
	float const S = 1.0f / (float)(sectors - 1);

	float x, y, z, texX, texY; //position and texture coordinates
	float pi = glm::pi<float>();
	glm::vec3 pos, norm; //position and normal of vertice

	for (int r = 0; r < rings; ++r) { //traverse over each division
		for (int s = 0; s < sectors; ++s) {


			//sphere calculationss
			x = glm::cos(2 * pi * s * S) * glm::sin(pi * r * R) * radius;
			y = glm::sin((pi * r * R) - (pi / 2.0f)) * radius;
			z = glm::sin(2.0f * pi * s * S) * glm::sin(pi * r * R) * radius;

			texY = (float)s / (float)sectors;

			int currRow = r * sectors;
			int nextRow = (r + 1) * sectors;

			pos = glm::vec3(x, y, z);
			norm = glm::normalize(pos);

			Vertex vertex;
			vertex.pos = pos;
			vertex.norm = norm;
			

			texX = norm.x / 2.0f + 0.5f;
			texY = norm.y / 2.0f + 0.5f;

			vertex.tex_coords = glm::vec2(texX, texY);

			mesh_vertices.push_back(vertex);
			
			mesh_indices.push_back(currRow + s);
			mesh_indices.push_back(nextRow + s);
			mesh_indices.push_back(nextRow + (s + 1));
			mesh_indices.push_back(currRow + s);
			mesh_indices.push_back(nextRow + (s + 1));
			mesh_indices.push_back(currRow + (s + 1));
		}
	}

	numIndices = (unsigned int) mesh_indices.size();
}

