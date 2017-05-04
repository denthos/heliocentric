#include "skybox_mesh.h"
#include "client.h"
#define PROJECTION_UNIFORM "projection"
#define VIEW_UNIFORM "view"
#define MTL_UNIFORM "material"
#define VIEWPOS_UNIFORM "viewPos"
#define MODEL_UNIFORM "model"
#define CUBEMAP_UNIFORM "cubemap"

SkyboxMesh::SkyboxMesh(const char* right, const char* left, const char* up, const char* down, const char* back, const char* front)
{
	//store the names of the texture files
	textureFiles.push_back(right);
	textureFiles.push_back(left);
	textureFiles.push_back(up);
	textureFiles.push_back(down);
	textureFiles.push_back(back);
	textureFiles.push_back(front);

	skyboxTextureID = loadCubemap(); //save cubemap id

	genMesh(); //generate skybox vertice info
	createMesh();
}

SkyboxMesh::~SkyboxMesh()
{
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

//TODO: fix vertices. textures are mirrored or rotated. temporarily rotated them manually to make them match with cubemap
void SkyboxMesh::genMesh()
{
	mesh_indices = { //order of vertices
		0,1,2, //back (direction user is facing initially)
		2,3,0,

		6,5,4,//front (behind user initially)
		4,7,6,

		10,9,8,
		8,11,10, //left

		14,13,12,
		12,15,14, //right

		18,17,16,
		16,19,18, //bottom

		22,23,20,
		20,21,22 //top	
	};

	//coordinates for the corners of a unit cube centered at origin
	glm::vec3 backBottomLH(-0.5f);
	glm::vec3 backBottomRH(0.5f, -0.5f, -0.5f);
	glm::vec3 backTopRH(0.5f, 0.5f, -0.5f);
	glm::vec3 backTopLH(-0.5f, 0.5f, -0.5f);

	glm::vec3 frontBottomLH = backBottomLH * glm::vec3(1.0f, 1.0f, -1.0f);
	glm::vec3 frontBottomRH = backBottomRH * glm::vec3(1.0f, 1.0f, -1.0f);
	glm::vec3 frontTopRH = backTopRH * glm::vec3(1.0f, 1.0f, -1.0f);
	glm::vec3 frontTopLH = backTopLH * glm::vec3(1.0f, 1.0f, -1.0f);

	//construct vertex vector
	Vertex vertex;

	//back face
	vertex.pos = backBottomLH; //bottom left 0
	mesh_vertices.push_back(vertex);

	vertex.pos = backBottomRH; //bottom right 1
	mesh_vertices.push_back(vertex);

	vertex.pos = backTopRH; //top right 2
	mesh_vertices.push_back(vertex);

	vertex.pos = backTopLH; //top left 3
	mesh_vertices.push_back(vertex);

	//front face
	vertex.pos = frontBottomLH; //bottom left 4
	mesh_vertices.push_back(vertex);

	vertex.pos = frontBottomRH; //bottom right 5
	mesh_vertices.push_back(vertex);

	vertex.pos = frontTopRH; //top right 6
	mesh_vertices.push_back(vertex);

	vertex.pos = frontTopLH; //top left 7
	mesh_vertices.push_back(vertex);

	//left face
	vertex.pos = backBottomLH; //bottom left 8
	mesh_vertices.push_back(vertex);

	vertex.pos = frontBottomLH; //bottom right 9
	mesh_vertices.push_back(vertex);

	vertex.pos = frontTopLH; //top right 10
	mesh_vertices.push_back(vertex);

	vertex.pos = backTopLH; //top left 11
	mesh_vertices.push_back(vertex);

	//right face
	vertex.pos = frontBottomRH; //bottom left 12
	mesh_vertices.push_back(vertex);

	vertex.pos = backBottomRH; //bottom right 13
	mesh_vertices.push_back(vertex);

	vertex.pos = backTopRH; //top right 14
	mesh_vertices.push_back(vertex);

	vertex.pos = frontTopRH; //top left 15
	mesh_vertices.push_back(vertex);

	//bottom face
	vertex.pos = backBottomLH; //bottom left 16
	mesh_vertices.push_back(vertex);

	vertex.pos = backBottomRH; //bottom right 17
	mesh_vertices.push_back(vertex);

	vertex.pos = frontBottomRH; //top right 18
	mesh_vertices.push_back(vertex);

	vertex.pos = frontBottomLH; //top left 19
	mesh_vertices.push_back(vertex);

	//top face
	vertex.pos = backTopLH; //bottom left 20
	mesh_vertices.push_back(vertex);

	vertex.pos = backTopRH; //bottom right 21
	mesh_vertices.push_back(vertex);

	vertex.pos = frontTopRH; //top right 22
	mesh_vertices.push_back(vertex);

	vertex.pos = frontTopLH; //top left 23
	mesh_vertices.push_back(vertex);
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
	glBindVertexArray(VAO);

	//draw
	glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

	//reset
	glBindVertexArray(0);
	shader.unbind();

	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
}