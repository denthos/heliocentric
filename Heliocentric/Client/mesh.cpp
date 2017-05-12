#include "mesh.h"
#include "client.h"

#include<iostream>

#define PROJECTION_UNIFORM "projection"
#define VIEW_UNIFORM "view"
#define MTL_UNIFORM "material"
#define VIEWPOS_UNIFORM "viewPos"
#define MODEL_UNIFORM "model"


#include "camera.h"
#include "logging.h"

Mesh::Mesh(MeshGeometry* geom) : geometry(geom) {

}

void Mesh::update() {

}

//TODO: separate binding functions for each mesh component? bind texture, bind material, bind light
void Mesh::draw(const Shader & shader, const Camera & camera, const glm::mat4 & toWorld)
{
	//which diffuse and specular samplers
	GLuint diffuse_num = 1;
	GLuint spec_num = 1;

	GLuint shaderID = shader.getPid(); //shader program number


	shader.bind(); //start using our shader

				   //pass camera, window matrices, and model's world matrix info to shader uniform variables
	glUniformMatrix4fv(glGetUniformLocation(shaderID, PROJECTION_UNIFORM), 1, GL_FALSE, &camera.perspective[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(shaderID, VIEW_UNIFORM), 1, GL_FALSE, &camera.view[0][0]);

	glUniform3f(glGetUniformLocation(shaderID, VIEWPOS_UNIFORM), camera.position.x, camera.position.y, camera.position.z);


	//TODO: bind lights -- naming convention for lights in shader, make UBO for light types. this should probably be moved made its own class or so
	glUniform3f(glGetUniformLocation(shader.getPid(), "pointLight.position"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(shader.getPid(), "pointLight.ambient"), 0.3f, 0.3f, 0.3f);
	glUniform3f(glGetUniformLocation(shader.getPid(), "pointLight.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(shader.getPid(), "pointLight.specular"), 0.7f, 0.7f, 0.7f);
	glUniform1f(glGetUniformLocation(shader.getPid(), "pointLight.quadratic"), 0.00030f);
	glUniform1f(glGetUniformLocation(shader.getPid(), "pointLight.linear"), 0.0002f);
	glUniform1f(glGetUniformLocation(shader.getPid(), "pointLight.constant"), 0.00050f);
	
	// Add time component to geometry shader in the form of a uniform
	glUniform1f(glGetUniformLocation(shader.getPid(), "time"), glfwGetTime() - creationTime);

	glUniformMatrix4fv(glGetUniformLocation(shaderID, MODEL_UNIFORM), 1, GL_FALSE, &toWorld[0][0]);


	//bind material
	glUniformBlockBinding(shaderID, glGetUniformBlockIndex(shaderID, MTL_UNIFORM), 1);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, UBO);

	//bind textures
	for (GLuint i = 0; i < mesh_textures.size(); i++) {
//set which texture we're using
glActiveTexture(GL_TEXTURE0 + i);

		std::string tex_name = mesh_textures[i]->type;
		std::string tex_num = (tex_name == "texture_diffuse") ? std::to_string(diffuse_num++) : std::to_string(spec_num++);

		//bind texture
		glUniform1i(glGetUniformLocation(shaderID, (tex_name + tex_num).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, mesh_textures[i]->id);
	}


	render();

	//reset everything
	for (GLuint i = 0; i < mesh_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//shader.unbind();
}

void Mesh::setTexture(const Texture* texture) {
	if (mesh_textures.size()) {
		mesh_textures[0] = texture;
	}
	else {
		mesh_textures.push_back(texture);
	}
}

BoundingBox Mesh::getBoundingBox() const {
	return geometry->boundingBox;
}

void Mesh::createMesh()
{
	/* Create the geometry */
	geometry->createGeometry();

	/* Do material stuff */
	glGenBuffers(1, &UBO);

	//vertex  material
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Material), (GLvoid*)&mtl, GL_STATIC_DRAW);
}

void Mesh::render() {
	//bind vao 
	glBindVertexArray(geometry->VAO);

	//draw
	glDrawElements(GL_TRIANGLES, (GLsizei)geometry->indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}