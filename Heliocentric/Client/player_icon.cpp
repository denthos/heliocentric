#include "player_icon.h"
#include <math.h>
PlayerIcon::PlayerIcon(Shader * shader)
{
	this->shader = shader;
	this->world_mat = glm::mat4(1.0f);

	//generate buffer info
	// Vertex data
	float points[] = {
		0.0f,  0.0f, 1.0f, 1.0f, 1.0f, 1.0, // top-left
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

void PlayerIcon::update(const glm::vec3 & position)
{
	world_mat = glm::translate(glm::mat4(1.0f), position + unit_offset);
}


void PlayerIcon::draw(const Camera & camera)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->bind();
	GLuint shaderID = shader->getPid();  

	//get clipping coordinates which are normalized between -1 and 1
	glm::vec4 viewportProjection = (camera.perspective  * camera.view * world_mat * glm::vec4(glm::vec3(0.0), 1.0f));

	
	glm::vec3 offset = viewportProjection / viewportProjection.w;

	glUniform3fv(glGetUniformLocation(shaderID, "player_color"), 1, &color[0]);
	glUniform3fv(glGetUniformLocation(shaderID, "offset"), 1, &offset[0]);


	//bind texture
	//texture.bind();
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 1);

	glBindVertexArray(0);
	//texture.unbind();

	shader->unbind();
}

void PlayerIcon::setColor(glm::vec4 color)
{
	this->color = color;
}
