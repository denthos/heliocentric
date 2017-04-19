#pragma once


#include "GL\glew.h"
#include "soil.h"

#include <assimp\scene.h> //ai textures
#include <assimp\postprocess.h>

class Texture  {
public:

	GLuint id;
	std::string type; //either diffuse or specular
	

	Texture();
	Texture(const char* file);
	Texture(const char* file, std::string type);
	~Texture() {}

	void bind();
	void unbind();

private:
	void load(const char* file);

	
};

