#pragma once

#include "model.h"
#include "bounding_box.h"
#include <memory>

class Drawable {
public:
	Drawable();
	virtual ~Drawable();
	virtual void draw(const Shader &, const Camera &) const;
	virtual void update() = 0;
	virtual BoundingBox getBoundingBox() const;
	virtual bool intersect(const Ray &, Collision &) const;
	virtual bool do_animation(const Shader &, const Camera &) const { return true; }

	const glm::mat4& getToWorld() const;

protected:
	Model * model;
	glm::mat4 toWorld;
	//glm::mat4 rotation;
};