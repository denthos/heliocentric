#include "file_mesh.h"
#include "client.h"
#include <iostream>




FileMesh::FileMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<const Texture*> textures, Material materials)
{
	
	mesh_vertices = vertices;
	mesh_indices = indices;
	mesh_textures = textures;
	mtl = materials;

	createMesh();
}

FileMesh::~FileMesh()
{
}







