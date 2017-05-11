#include "quad_mesh.h"

QuadMesh::QuadMesh()
{
	genMesh();

}

void QuadMesh::Draw(Shader & shader)
{
	shader.bind();

	Draw();

	shader.unbind();
}

void QuadMesh::Draw()
{
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void QuadMesh::genMesh()
{
	Vertex vertex;

	//fill in normalized quad position vectors
	vertex.pos = glm::vec3(-1.0f, 1.0f, 0.0f);
	vertex.tex_coords = glm::vec2(0.0f,1.0f);
	mesh_vertices.push_back(vertex);

	vertex.pos = glm::vec3(-1.0f, -1.0f, 0.0f);
	vertex.tex_coords = glm::vec2(0.0f, 0.0f);
	mesh_vertices.push_back(vertex);

	vertex.pos = glm::vec3(1.0f, 1.0f, 0.0f);
	vertex.tex_coords = glm::vec2(1.0f, 1.0f);
	mesh_vertices.push_back(vertex);

	vertex.pos = glm::vec3(1.0f, -1.0f, 0.0f);
	vertex.tex_coords = glm::vec2(1.0f, 0.0f);
	mesh_vertices.push_back(vertex);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh_vertices.size() * sizeof(Vertex), &mesh_vertices.at(0), GL_STATIC_DRAW);

	//vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	//vertex texture coordinates
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex_coords));


	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
