#ifndef SPHERE_H_
#define SPHERE_H_

#include "Drawable.h"

class Sphere : public Drawable {
public:
	static bool init;
	static GLuint VBO, VAO, EBO, VN, TO;
	static unsigned int numIndices;

	Sphere();
private:
	virtual void render();
};


#endif