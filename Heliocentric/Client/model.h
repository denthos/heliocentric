#pragma once


#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <soil.h>
#include "file_mesh.h"
#include "camera.h"
#include "singleton.h"

#include <string>
#include <iostream>

class Model : public KeyedSingleton<Model, std::string> {

	friend class KeyedSingleton<Model, std::string>;

public:
	Model() {}

	void draw(const Shader & shader, const Camera & camera, const glm::mat4 & toWorld);
	void update();
	BoundingBox getBoundingBox();
protected:
	BoundingBox boundingBox;

	std::vector<Mesh> meshes; //meshes in the model
	void calculateBoundingBox();
private:
	Model(std::string file);
	std::string directory; //folder that model files belong in
	void load(std::string file); //load the model file info
	void processNode(aiNode* node, const aiScene* scene); 
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<const Texture*> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};

