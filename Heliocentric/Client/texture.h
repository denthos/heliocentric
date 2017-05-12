#pragma once


#include "GL\glew.h"
#include "soil.h"

#include <assimp\scene.h> //ai textures
#include <assimp\postprocess.h>

#include "singleton.h"


class Texture  : public KeyedSingleton<Texture, std::string> {

	friend class KeyedSingleton<Texture, std::string>;

public:
	GLuint id;
	std::string type; //either diffuse or specular

	/* Don't allow anyone to copy textures. If they are trying to copy, move instead. */
	Texture(const Texture&) = delete;
	Texture(Texture&&) = default;
	
	~Texture() {}

	void bind();
	void unbind();

private:
	void load(std::string filename);

	Texture(std::string filename);
	Texture(std::string filename, std::string type);

};

