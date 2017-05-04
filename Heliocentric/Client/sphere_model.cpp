#include "sphere_model.h"

#include "sphere_mesh.h"

SphereModel::SphereModel(const Texture* tex) {
	meshes.push_back(SphereMesh());
	meshes[0].setTexture(tex);
}