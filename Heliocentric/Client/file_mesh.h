#pragma once


#include "mesh.h"

class FileMesh : public Mesh
{
public:
	FileMesh(MeshGeometry* geom, std::vector<const Texture*> textures, Material materials);
};




