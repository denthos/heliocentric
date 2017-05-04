#pragma once
#include <assimp\postprocess.h>
#include "shader.h"
#include "vertex.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "texture.h"
#include "camera.h"
#include "bounding_box.h"

#include <string>
#include <vector>
#include <memory>

//the material properties for a mesh
struct Material {
	glm::vec3 diffuse, ambient, specular, emission; //colors
	float shininess;
};

//abstract class for all meshes
class Mesh
{
public:

	virtual void update();

	virtual void draw(const Shader & shader, const Camera & camera, const glm::mat4 & toWorld); //draws mesh

	virtual void setTexture(Texture & texture);
	
	BoundingBox getBoundingBox() const;

	std::vector<GLuint> mesh_indices;
	std::vector<Vertex> mesh_vertices; //mesh vertices and their info
	std::vector<Texture> mesh_textures;

	Material mtl; //mesh material

	//TODO set material functions
	//TODO add Texture

protected:
	//vertex array, vertex buffer, uniform buffer
	GLuint VAO, VBO, EBO, UBO;
	BoundingBox boundingBox;

	void createMesh(); //opengl binding of mesh data
	virtual void genMesh();
};
