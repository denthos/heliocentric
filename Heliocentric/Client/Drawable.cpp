#include "Drawable.h"

#include "Client.h"

void Drawable::draw(GLuint shader, glm::mat4 C) {
	glUseProgram(shader);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &Client::perspectiveMatrix[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &Client::viewMatrix[0][0]);
	glUniform3f(glGetUniformLocation(shader, "viewPos"), Client::camPos.x, Client::camPos.y, Client::camPos.z);
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &C[0][0]);

	glUniform1f(glGetUniformLocation(shader, "material.shininess"), 0.2f);
	glUniform3f(glGetUniformLocation(shader, "material.ambient"), 0.3f, 0.3f, 0.3f);
	glUniform3f(glGetUniformLocation(shader, "material.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(shader, "material.specular"), 0.0f, 0.0f, 0.0f);

	glUniform3f(glGetUniformLocation(shader, "pointLight.position"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(shader, "pointLight.ambient"), 0.3f, 0.3f, 0.3f);
	glUniform3f(glGetUniformLocation(shader, "pointLight.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(shader, "pointLight.specular"), 0.7f, 0.7f, 0.7f);
	glUniform1f(glGetUniformLocation(shader, "pointLight.quadratic"), 0.00032f);

	render();
}

void Drawable::update() {

}