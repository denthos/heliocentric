#include "assimp_geometry.h"

AssimpGeometry::AssimpGeometry(aiMesh * mesh) : mesh(mesh) {

}

void AssimpGeometry::generateGeometry(std::vector<Vertex> & vertices, std::vector<GLuint> & indices) {
	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		//get position
		aiVector3D pos = mesh->mVertices[i];
		vertex.pos = glm::vec3(pos.x, pos.y, pos.z);


		//get normals
		aiVector3D norm = mesh->mNormals[i];
		vertex.norm = glm::vec3(norm.x, norm.y, norm.z);

		//get texture coordinates
		if (mesh->mTextureCoords[0]) {
			aiVector3D tex = mesh->mTextureCoords[0][i];
			vertex.tex_coords = glm::vec2(tex.x, tex.y);

		}
		else { vertex.tex_coords = glm::vec2(0.0f, 0.0f); }

		vertices.push_back(vertex);
	}

	//get indices
	for (GLuint i = 0; i < mesh->mNumFaces; i++) {

		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}
}