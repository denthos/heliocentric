#include "quad.h"

#include "vertex.h"

GLuint Quad::VAO, Quad::VBO;
bool Quad::initialized = false;

Quad::Quad() {
	if (!initialized) {
		Vertex vertex;
		Vertex vertices[4];
		//fill in normalized quad position vectors
		vertex.pos = glm::vec3(-1.0f, 1.0f, 0.0f);
		vertex.tex_coords = glm::vec2(0.0f, 1.0f);
		vertices[0] = vertex;

		vertex.pos = glm::vec3(-1.0f, -1.0f, 0.0f);
		vertex.tex_coords = glm::vec2(0.0f, 0.0f);
		vertices[1] = vertex;

		vertex.pos = glm::vec3(1.0f, 1.0f, 0.0f);
		vertex.tex_coords = glm::vec2(1.0f, 1.0f);
		vertices[2] = vertex;

		vertex.pos = glm::vec3(1.0f, -1.0f, 0.0f);
		vertex.tex_coords = glm::vec2(1.0f, 0.0f);
		vertices[3] = vertex;

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex), vertices, GL_STATIC_DRAW);

		//vertex positions
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

		//vertex texture coordinates
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex_coords));

		//unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		initialized = true;
	}
}

void Quad::draw(const Shader & shader) {
	shader.bind();
	draw();
	shader.unbind();
}

void Quad::draw() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}