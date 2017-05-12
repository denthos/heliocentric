#include "file_mesh.h"
#include "client.h"
#include <iostream>


FileMesh::FileMesh(MeshGeometry* geom, std::vector<const Texture*> textures, Material materials) :
	Mesh(geom)
{

	mtl = materials;
	createMesh();
}