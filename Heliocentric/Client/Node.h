#ifndef NODE_H_
#define NODE_H_

#include <GL\glew.h>
#include <glm\mat4x4.hpp>

class Node {
public:
	virtual void draw(GLuint shader, glm::mat4) = 0;
	virtual void update() = 0;
};

#endif