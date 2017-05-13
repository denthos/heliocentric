#pragma once

#include "mesh_geometry.h"
#include "singleton.h"
#include <assimp\scene.h>

class AssimpGeometry : public MeshGeometry {
public:
	AssimpGeometry(aiMesh * mesh);
	virtual void generateGeometry(std::vector<Vertex> & vertices, std::vector<GLuint> & indices);
private:
	aiMesh * mesh;

};