#pragma once
#include <vector>
#include <GL/glew.h>
#include <atomic>

#include "vertex.h"
#include "bounding_box.h"

class MeshGeometry {
public:

	virtual void generateGeometry(std::vector<Vertex> & vertices, std::vector<GLuint> & indices) = 0;

	std::atomic_bool geometry_created = false;
	virtual void createGeometry();

	GLuint VAO, VBO, EBO;
	GLsizei numIndices;
	BoundingBox boundingBox;
};