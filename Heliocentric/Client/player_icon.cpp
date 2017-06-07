#include "player_icon.h"

PlayerIcon::PlayerIcon(Shader * shader)
{
	this->shader = shader;
	this->world_mat = glm::mat4(1.0f);

	//generate buffer info
	// Vertex data
	float points[] = {
		0.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0, // top-left
	};
	
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(2 * sizeof(float)));
	glBindVertexArray(0);


	//TODO: initialize icon

}

PlayerIcon::~PlayerIcon()
{
}


void PlayerIcon::draw(const Camera & camera, const glm::mat4 & toWorld)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->bind();
	GLuint shaderID = shader->getPid();

	glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, &camera.perspective[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "view"), 1, GL_FALSE, &camera.view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "model"), 1, GL_FALSE, &toWorld[0][0]);




	//bind texture
	//texture.bind();
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 1);

	glBindVertexArray(0);
	//texture.unbind();

	shader->unbind();
}
