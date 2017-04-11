#ifndef APPLYTEXTURE_H_
#define APPLYTEXTURE_H_

#include "Group.h"

class ApplyTexture : public Group {
public:
	ApplyTexture(GLuint);
	virtual void draw(GLuint, glm::mat4);
protected:
	static bool init;
	static GLuint textureShader;
	GLuint texture;
};

#endif