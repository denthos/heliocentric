#include "sphere_model.h"

#include "sphere_mesh.h"

SphereModel::SphereModel(const Texture* tex) {
	this->meshes.push_back(SphereMesh());
	this->meshes[0].setTexture(tex);
	this->calculateBoundingBox();
}