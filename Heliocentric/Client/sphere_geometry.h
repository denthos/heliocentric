#pragma once
#include "mesh_geometry.h"
#include "singleton.h"

class SphereMeshGeometry : public MeshGeometry, public Singleton<SphereMeshGeometry> {

	friend class Singleton<SphereMeshGeometry>;

private:
	int rings, sectors;
	SphereMeshGeometry(int rings, int sectors);
	SphereMeshGeometry();

public:
	void generateGeometry();
};