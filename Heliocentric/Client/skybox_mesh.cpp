#include "skybox_mesh.h"
#include "client.h"
#define PROJECTION_UNIFORM "projection"
#define VIEW_UNIFORM "view"
#define MTL_UNIFORM "material"
#define VIEWPOS_UNIFORM "viewPos"
#define MODEL_UNIFORM "model"
#define CUBEMAP_UNIFORM "cubemap"

SkyboxMesh::SkyboxMesh(const char* right, const char* left, const char* up, const char* down, const char* back, const char* front, SkyboxMeshGeometry* geom) :
	Mesh(geom)
{
	//store the names of the texture files
	textureFiles.push_back(right);
	textureFiles.push_back(left);
	textureFiles.push_back(up);
	textureFiles.push_back(down);
	textureFiles.push_back(back);
	textureFiles.push_back(front);

	skyboxTextureID = loadCubemap(); //save cubemap id
	createMesh();
}

GLuint SkyboxMesh::loadCubemap()
{
	int width, height; //dimensions of texture image
	unsigned char* image;//current texture image

	GLuint textureID;//create a cubemap texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (GLuint i = 0; i < textureFiles.size(); i++)
	{
		image = SOIL_load_image(textureFiles[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}


void SkyboxMesh::draw(const Shader & shader, const Camera & camera, const glm::mat4 & toWorld) {
	GLuint shaderID = shader.getPid(); //shader program number
	shader.bind(); //start using our shader

	glDisable(GL_DEPTH_TEST);
	glDepthMask(false);

				   //pass camera, window matrices, and model's world matrix info to shader uniform variables
	glUniformMatrix4fv(glGetUniformLocation(shaderID, PROJECTION_UNIFORM), 1, GL_FALSE, &camera.infinite_perspective[0][0]);

	glUniformMatrix4fv(glGetUniformLocation(shaderID, VIEW_UNIFORM), 1, GL_FALSE, &camera.view[0][0]);

	glUniform3f(glGetUniformLocation(shaderID, VIEWPOS_UNIFORM), 0.0,0.0,0.0);

	glUniformMatrix4fv(glGetUniformLocation(shaderID, MODEL_UNIFORM), 1, GL_FALSE, &toWorld[0][0]);

	//bind cubemap texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
	glUniform1i(glGetUniformLocation(shaderID, CUBEMAP_UNIFORM), 0);

	//bind vao 
	glBindVertexArray(geometry->VAO);

	//draw
	glDrawElements(GL_TRIANGLES, geometry->numIndices, GL_UNSIGNED_INT, 0);

	//reset
	glBindVertexArray(0);
	shader.unbind();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);

}