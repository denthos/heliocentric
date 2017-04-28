#pragma once

#include <GL\glew.h>
#include <glm\mat4x4.hpp>
#include "bounding_box.h"
#include "camera.h"

class Drawable {
public:
	glm::mat4 toWorld;
	BoundingBox boundingBox;

	virtual void draw(GLuint shader, const Camera & camera) {
		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &toWorld[0][0]);
		render();
	}
protected:
	~Drawable() {}

private:
	virtual void render() {}
};