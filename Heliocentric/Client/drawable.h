#pragma once

#include "model.h"
#include "bounding_box.h"

class Drawable {
public:
	Drawable();
	virtual void draw(const Shader &, const Camera &);
	virtual void update() = 0;
	virtual const BoundingBox & getBoundingBox();
protected:
	Mesh * mesh;
	glm::mat4 toWorld;
};