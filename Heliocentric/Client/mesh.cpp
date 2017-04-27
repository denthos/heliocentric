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


void Mesh::Update(glm::mat4 & parent)
{
	world_mat = glm::mat4(1.0f);
	world_mat = world_mat * scale_mat;
	world_mat = world_mat * rot_mat;
	world_mat = world_mat * trans_mat;
	

	world_mat = parent * world_mat;
}

//TODO: separate binding functions for each mesh component? bind texture, bind material, bind light
void Mesh::Draw(Shader &shader, const Camera & camera)
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

	glUniformMatrix4fv(glGetUniformLocation(shaderID, MODEL_UNIFORM), 1, GL_FALSE, &world_mat[0][0]);

	//TODO: bind lights -- naming convention for lights in shader, make UBO for light types. this should probably be moved made its own class or so
	glUniform3f(glGetUniformLocation(shader.getPid(), "pointLight.position"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(shader.getPid(), "pointLight.ambient"), 0.3f, 0.3f, 0.3f);
	glUniform3f(glGetUniformLocation(shader.getPid(), "pointLight.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(shader.getPid(), "pointLight.specular"), 0.7f, 0.7f, 0.7f);
	glUniform1f(glGetUniformLocation(shader.getPid(), "pointLight.quadratic"), 0.00032f);

	//bind material
	glUniformBlockBinding(shaderID, glGetUniformBlockIndex(shaderID, MTL_UNIFORM), 0);

	glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, sizeof(Material));
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);

	//bind textures
	for (GLuint i = 0; i < mesh_textures.size(); i++) {
		//set which texture we're using
		glActiveTexture(GL_TEXTURE0 + i);

		std::string tex_name = mesh_textures[i].type;
		std::string tex_num = (tex_name == "texture_diffuse") ? std::to_string(diffuse_num++) : std::to_string(spec_num++);

		//bind texture
		glUniform1i(glGetUniformLocation(shaderID, (tex_name + tex_num).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, mesh_textures[i].id);
	}

	

	//bind vao 
	glBindVertexArray(VAO);

	//draw
	glDrawElements(GL_TRIANGLES, (GLsizei) mesh_indices.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

	//reset everything
	for (GLuint i = 0; i < mesh_textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//shader.unbind();
}


void Mesh::setScale(glm::mat4 scale)
{
	scale_mat = scale;
}

void Mesh::createMesh()
{
	//set up buffers and specify vertex shader

	//generate vertex arrays and buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &UBO);

	//bind values to array and buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, mesh_vertices.size() * sizeof(Vertex), &mesh_vertices.at(0), GL_STATIC_DRAW);



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_indices.size() * sizeof(GLuint), &mesh_indices.at(0), GL_STATIC_DRAW);

	//vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	//vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, norm));

	//vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex_coords));


	//vertex  material
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Material), (GLvoid*)&mtl, GL_STATIC_DRAW);

	//Report any errors
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		Lib::LOG_ERR("Error while creating mesh!");
	}

	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);




}
