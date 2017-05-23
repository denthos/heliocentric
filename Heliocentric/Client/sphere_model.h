#pragma once

#include "model.h"
#include "texture.h"

class SphereModel : public Model {
public:
	SphereModel(const Texture*);
	virtual bool intersect(const Ray & ray, Collision & collision) const;
};