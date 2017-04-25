#pragma once


#include "mesh.h"




class FileMesh : public Mesh
{
public:

	FileMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, Material materials);
	~FileMesh();


};




