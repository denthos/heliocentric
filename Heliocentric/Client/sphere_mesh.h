#pragma once

#include "mesh.h"



class SphereMesh :public Mesh{
public:
	SphereMesh();
	SphereMesh(float radius, int rings, int sectors);

	~SphereMesh();

	static bool init;

private:

	
	unsigned int numIndices;

	float radius;
	int rings, sectors;

	virtual void genMesh(); //generates data to be used in mesh



};

