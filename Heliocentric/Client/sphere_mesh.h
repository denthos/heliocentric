#pragma once

#include "mesh.h"



class SphereMesh :public Mesh{
public:
	SphereMesh();
	SphereMesh(float radius, int rings, int sectors);

	~SphereMesh();

	void setTexture(Texture texture);

	static bool init;

private:

	
	unsigned int numIndices;

	float radius;
	int rings, sectors;

	void genMesh(); //generates data to be used in mesh



};

