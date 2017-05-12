#pragma once

#include "shader.h"

class Quad {
public:
	Quad();
	void draw(const Shader & shader);
	void draw();
private:
	static GLuint VAO, VBO;
	static bool initialized;
};