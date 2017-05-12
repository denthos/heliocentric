#include "sphere_geometry.h"

SphereMeshGeometry::SphereMeshGeometry() : SphereMeshGeometry(24, 48) {}

SphereMeshGeometry::SphereMeshGeometry(int rings, int sectors) :
	rings(rings), sectors(sectors) {}

void SphereMeshGeometry::generateGeometry() {
	//generate data for sphere mesh data buffers
	float const R = 1.0f / (float)(rings - 1); //what fraction of the sphere each ring division is
	float const S = 1.0f / (float)(sectors - 1);

	float x, y, z, texX, texY; //position and texture coordinates
	float pi = glm::pi<float>();
	glm::vec3 pos, norm; //position and normal of vertice

	for (int r = 0; r < rings; ++r) { //traverse over each division
		for (int s = 0; s < sectors; ++s) {


			//sphere calculationss
			x = glm::cos(2 * pi * s * S) * glm::sin(pi * r * R);
			y = glm::sin((pi * r * R) - (pi / 2.0f));
			z = glm::sin(2.0f * pi * s * S) * glm::sin(pi * r * R);

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

			vertices.push_back(vertex);
			
			indices.push_back(currRow + s);
			indices.push_back(nextRow + s);
			indices.push_back(nextRow + (s + 1));
			indices.push_back(currRow + s);
			indices.push_back(nextRow + (s + 1));
			indices.push_back(currRow + (s + 1));
		}
	}
}