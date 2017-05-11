#pragma once
#include "mesh.h"

class SkyboxMesh :public Mesh {
public:
	SkyboxMesh(const char* right, const char* left, const char* up, const char* down, const char* back, const char* front);

	~SkyboxMesh();

	virtual void draw(const Shader &shader, const Camera & camera, const glm::mat4 & toWorld);

private:
	std::vector<const GLchar*> textureFiles; //the name of the 6 side textures
	GLuint skyboxTextureID;

	GLuint loadCubemap();
	void genMesh(); //generates data to be used in mesh
};

