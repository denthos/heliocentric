#include "sphere.h"

#include "client.h"

#include <glm\trigonometric.hpp>
#include <glm\gtc\constants.hpp>
#include <vector>

#define RADIUS 1.0f
#define RINGS 24
#define SECTORS 48

bool Sphere::init = false;
GLuint Sphere::VBO, Sphere::VAO, Sphere::EBO, Sphere::VN, Sphere::TO;
unsigned int Sphere::numIndices;

Sphere::Sphere() {
	if (!init) {
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> texCoords;
		std::vector<GLuint> indices;
		float const R = 1.0f / (float)(RINGS - 1);
		float const S = 1.0f / (float)(SECTORS - 1);
		float x, y, z, texX, texY;
		float pi = glm::pi<float>();
		glm::vec3 pos, norm;
		for (int r = 0; r < RINGS; ++r) {
			for (int s = 0; s < SECTORS; ++s) {

				x = glm::cos(2 * pi * s * S) * glm::sin(pi * r * R) * RADIUS;
				y = glm::sin((pi * r * R) - (pi / 2.0f)) * RADIUS;
				z = glm::sin(2.0f * pi * s * S) * glm::sin(pi * r * R) * RADIUS;

				texY = (float)s / (float)SECTORS;

				int currRow = r * SECTORS;
				int nextRow = (r + 1) * SECTORS;

				pos = glm::vec3(x, y, z);
				norm = glm::normalize(pos);

				//texX = glm::asin(norm.x) / glm::pi<float>() + 0.5f;
				//texY = glm::asin(norm.y) / glm::pi<float>() + 0.5f;

				texX = norm.x / 2.0f + 0.5f;
				texY = norm.y / 2.0f + 0.5f;

				vertices.push_back(pos);
				normals.push_back(norm);
				texCoords.push_back(glm::vec2(texX, texY));
				indices.push_back(currRow + s);
				indices.push_back(nextRow + s);
				indices.push_back(nextRow + (s + 1));
				indices.push_back(currRow + s);
				indices.push_back(nextRow + (s + 1));
				indices.push_back(currRow + (s + 1));
			}
		}

		numIndices = indices.size();

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenBuffers(1, &VN);
		glGenBuffers(1, &TO);

		glBindVertexArray(VAO);

		// Vertices
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		// Normals
		glBindBuffer(GL_ARRAY_BUFFER, VN);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		// Texture Coordinates
		glBindBuffer(GL_ARRAY_BUFFER, TO);
		glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec2), &texCoords[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);

		// Indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		init = true;
	}
}

void Sphere::draw(GLuint shader, glm::mat4 C) {
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

void Sphere::render() {
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}