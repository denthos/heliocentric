#pragma once

#include "mesh.h"
#include "sphere_geometry.h"


class SphereMesh : public Mesh{
public:
	SphereMesh(SphereMeshGeometry* geom);
};

