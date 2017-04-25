#pragma once
#include <assimp\postprocess.h>
#include "shader.h"
#include "vertex.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "texture.h"
#include "camera.h"

#include <string>

#include <vector>

//the material properties for a mesh
struct Material {
	glm::vec3 diffuse, ambient, specular, emission; //colors
	float shininess;
};

//abstract class for all meshes
class Mesh
{
public:

	glm::mat4 world_mat = glm::mat4(1.0f); //world matrix
	glm::mat4 scale_mat = glm::mat4(1.0f);
	glm::mat4 rot_mat = glm::mat4(1.0f);
	glm::mat4 trans_mat = glm::mat4(1.0f);

	void Update(glm::mat4 &parent); //TODO
	void Draw(Shader &shader, const Camera & camera); //draws mesh



	std::vector<GLuint> mesh_indices;
	std::vector<Vertex> mesh_vertices; //mesh vertices and their info
	std::vector<Texture> mesh_textures;

	Material mtl; //mesh material

	//TODO set material functions
	//TODO add Texture

	void setScale(glm::mat4 scale);

protected:
	//vertex array, vertex buffer, uniform buffer
	GLuint VAO, VBO, EBO, UBO;

	void createMesh(); //opengl binding of mesh data
};
