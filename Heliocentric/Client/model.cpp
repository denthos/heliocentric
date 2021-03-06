#define GLM_ENABLE_EXPERIMENTAL
#include "model.h"
#include "assimp_geometry.h"
#include <assimp\Importer.hpp>

Model::Model(std::string file)
{
	load(file);
	calculateBoundingBox();
}

void Model::draw(const Shader & shader, const Camera & camera, const glm::mat4 & toWorld) {
	for (GLuint i = 0; i < meshes.size(); i++) {
		meshes[i].draw(shader, camera, toWorld);
	}
}

void Model::update() {
	for (GLuint i = 0; i < meshes.size(); i++) {
		meshes[i].update();
	}
}

bool Model::intersect(const Ray & ray, Collision & collision) const {
	return boundingBox.intersect(ray, collision);
}

BoundingBox Model::getBoundingBox() const {
	return boundingBox;
}

void Model::load(std::string file)
{
	//load model into scene data structure (root)
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_FlipUVs);
	//optional options to look into: normals, split, optimize

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ASSIMP ERROR" << importer.GetErrorString() << std::endl;
		return;
	}

	directory = file.substr(0, file.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode * node, const aiScene * scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (GLuint i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh * mesh, const aiScene * scene)
{
	std::vector<const Texture*> textures;
	
	Material mtl;

	//get texture material
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<const Texture*> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<const Texture*> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texure_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		//get color material
		
		aiColor4D diffuse, ambient, specular, emission;
		
		float shininess = 0.0;
		unsigned int max;
		aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shininess, &max);
		mtl.shininess = shininess;

		if(AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse)) {
			mtl.diffuse = glm::vec3(diffuse.r, diffuse.g, diffuse.b);
		}

		if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_AMBIENT, ambient)) {
			mtl.ambient = glm::vec3(ambient.r, ambient.g, ambient.b);
		}

		if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, specular)) {
			mtl.specular = glm::vec3(specular.r, specular.g, specular.b);
		}

		if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_EMISSIVE, emission)) {
			mtl.emission = glm::vec3(emission.r, emission.g, emission.b);
		}

		
	}

	return FileMesh(new AssimpGeometry(mesh), textures, mtl);
}

std::vector<const Texture*> Model::loadMaterialTextures(aiMaterial * mat, aiTextureType type, std::string typeName)
{
	std::vector<const Texture*> textures;

	//check amount of textures stored in material
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {

		aiString str;

		//retrieve each texture file location
		mat->GetTexture(type, i, &str);
		std::string fp = directory + "/" + std::string(str.C_Str());
		
		textures.push_back(Texture::getInstance(fp, typeName));
	}

	return textures;
}

void Model::calculateBoundingBox() {
	boundingBox = BoundingBox(glm::vec3(FLT_MAX), glm::vec3(-FLT_MAX));
	for (unsigned int i = 0; i < meshes.size(); ++i) {
		boundingBox.expand(meshes[i].getBoundingBox());
	}
}