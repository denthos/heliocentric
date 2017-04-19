#pragma once


#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <soil.h>
#include "file_mesh.h"

#include <string>
#include <iostream>

class Model {
public:
	Model() {}
	Model(GLchar* file);
	void Draw(Shader &shader);
	void Update(glm::mat4 &parent);

	glm::mat4 world_mat;

	glm::mat4 scale_mat = glm::mat4(1.0f);

	void setScale(glm::mat4 scale);
private:
	std::vector<Mesh> meshes; //meshes in the model
	std::string directory; //folder that model files belong in

	void load(std::string file); //load the model file info
	void processNode(aiNode* node, const aiScene* scene); 
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

