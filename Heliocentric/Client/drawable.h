#pragma once

#include "model.h"
#include "bounding_box.h"

class Drawable {
public:
	Drawable();
	virtual ~Drawable() {}
	virtual void draw(const Camera &) const;
	virtual BoundingBox getBoundingBox() const;
	virtual bool intersect(const Ray &, Collision &) const;
	virtual bool do_animation(const Camera &) const;
	const glm::mat4& getToWorld() const;
	void mark_for_update();
	void performUpdate(bool force=false);

protected:
	Model * model;
	glm::mat4 toWorld;
	Shader * shader;
	bool needs_update;
	virtual void update() = 0;
};