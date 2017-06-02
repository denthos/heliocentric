#pragma once

#include "model.h"
#include "bounding_box.h"

class Drawable {
public:
	Drawable();
	virtual ~Drawable() {}
	virtual void draw(const Camera &) const;
	virtual void update() = 0;
	virtual BoundingBox getBoundingBox() const;
	virtual bool intersect(const Ray &, Collision &) const;

	const glm::mat4& getToWorld() const;

protected:
	Model * model;
	glm::mat4 toWorld;
	Shader * shader;
};