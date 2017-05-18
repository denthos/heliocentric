#pragma once
#include "mesh_geometry.h"

class SkyboxMeshGeometry : public MeshGeometry {

	virtual void generateGeometry(std::vector<Vertex> & vertices, std::vector<GLuint> & indices);
};