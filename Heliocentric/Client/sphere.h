#pragma once

#include <GL\glew.h>
#include <glm/mat4x4.hpp>

class Sphere {
public:
	static bool init;
	static GLuint VBO, VAO, EBO, VN, TO;
	static unsigned int numIndices;

	void draw(GLuint shader, glm::mat4 C);

	Sphere();
private:
	void render();
};