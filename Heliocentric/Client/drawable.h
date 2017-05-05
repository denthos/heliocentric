#pragma once

#include "model.h"
#include "bounding_box.h"
#include <memory>

class Drawable {
public:
	Drawable();
	virtual void draw(const Shader &, const Camera &) const;
	virtual void update() = 0;
	virtual BoundingBox getBoundingBox() const;

protected:
	Model * model;
	glm::mat4 toWorld;
};