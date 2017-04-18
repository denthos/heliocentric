#pragma once

#include <GL\glew.h>
#include <glm\mat4x4.hpp>

class Drawable {
public:
	glm::mat4 toWorld;

	virtual void draw(GLuint shader, glm::mat4 C) {
		glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &toWorld[0][0]);
		render();
	}
protected:
	~Drawable() {}

private:
	virtual void render() {}
};