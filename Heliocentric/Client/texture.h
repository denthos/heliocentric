#pragma once


#include "GL\glew.h"
#include "soil.h"

#include <assimp\scene.h> //ai textures
#include <assimp\postprocess.h>
#include <unordered_map>

class Texture  {
public:
	template <typename ... TConstructorArgs>
	static const Texture* getTexture(std::string filename, TConstructorArgs... args) {
		static std::unordered_map<std::string, Texture> textures;
		auto& texture_it = textures.find(filename);

		if (texture_it == textures.end()) {
			textures.insert(std::make_pair(filename, Texture(filename, args...)));
		}

		return &(textures.at(filename));
	}

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

