#pragma once
#include <assimp\postprocess.h>
#include "shader.h"
#include "vertex.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "texture.h"
#include "camera.h"
#include "bounding_box.h"
#include "mesh_geometry.h"

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
	Mesh(MeshGeometry* geometry);



	virtual void update();
	virtual void draw(const Shader & shader, const Camera & camera, const glm::mat4 & toWorld); //draws mesh
	virtual void render();
	virtual void setTexture(const Texture* texture);
	
	BoundingBox getBoundingBox() const;

	MeshGeometry* geometry;

	std::vector<const Texture*> mesh_textures;

	Material mtl; //mesh material

	//TODO set material functions
	//TODO add Texture
	float creationTime; //TODO this is temporary so i can test geoShader. remove later
	

protected:
	//vertex array, vertex buffer, uniform buffer
	GLuint UBO;

	void createMesh(); //opengl binding of mesh data
};
