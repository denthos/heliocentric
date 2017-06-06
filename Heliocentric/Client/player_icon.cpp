#include "player_icon.h"

PlayerIcon::PlayerIcon(Shader * shader)
{
	this->shader = shader;
	this->world_mat = glm::mat4(1.0f);

	//generate buffer info
	// Vertex data
	GLfloat points[] = {
		0.0f,  0.0f // Top-left
	};

	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); //layout 0: position
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);

	glGenBuffers(1, &icon_info_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, icon_info_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Icon), NULL, GL_STREAM_DRAW);

	glEnableVertexAttribArray(1); // layout 1: offset
	glBindBuffer(GL_ARRAY_BUFFER, icon_info_buffer);
	glVertexAttribPointer(
		1,
		3, // size : x + y + z 
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Icon), // stride
		(void*)0 // array buffer offset
	);

	glEnableVertexAttribArray(2); //layout 2: color
	glBindBuffer(GL_ARRAY_BUFFER, icon_info_buffer);
	glVertexAttribPointer(
		2,
		4, // size : rgba
		GL_FLOAT, // type
		GL_FALSE,
		sizeof(Icon),
		(GLvoid*)offsetof(Icon, color)
	);

	glEnableVertexAttribArray(3); //layout 3: size
	glBindBuffer(GL_ARRAY_BUFFER, icon_info_buffer);
	glVertexAttribPointer(
		3,
		1, // size 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Icon),
		(GLvoid*)offsetof(Icon, size)
	);

	glVertexAttribDivisor(0, 0); // particle center
	glVertexAttribDivisor(1, 1); // offset: one per quad
	glVertexAttribDivisor(2, 1); // color : one per quad -> 1
	glVertexAttribDivisor(3, 1); // size : one per quad -> 1



	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	//TODO: initialize icon
	icon.size = 10.0f;
	icon.color = glm::vec4(1.0);
	icon.pos = glm::vec3(0.0);
	


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

	glDrawArraysInstanced(GL_POINTS, 0, 1, 1);

	glBindVertexArray(0);
	//texture.unbind();

	shader->unbind();
}
