#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include "Node.h"

class Drawable : public Node {
public:
	virtual void draw(GLuint shader, glm::mat4);
	virtual void update();
private:
	virtual void render() = 0;
};

#endif