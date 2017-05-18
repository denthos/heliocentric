#include "mesh_geometry.h"
#include "logging.h"

void MeshGeometry::createGeometry() {
	if (geometry_created) {
		return;
	}

	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;

	generateGeometry(vertices, indices);

	//generate vertex arrays and buffers
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//bind values to array and buffers
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices.at(0), GL_STATIC_DRAW);



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices.at(0), GL_STATIC_DRAW);

	this->numIndices = (GLsizei)indices.size();

	//vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	//vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, norm));

	//vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, tex_coords));

	//Report any errors
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		LOG_ERR("Error while creating mesh!");
	}

	//unbind buffers
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// create bounding box around vertices
	boundingBox.min = glm::vec3(100000000.0f);
	boundingBox.max = glm::vec3(-100000000.0f);
	for (unsigned int i = 0; i < vertices.size(); ++i) {
		boundingBox.expand(vertices[i].pos);
	}

	geometry_created = true;
}