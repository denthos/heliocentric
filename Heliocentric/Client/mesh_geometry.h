#pragma once
#include <vector>
#include <GL/glew.h>
#include <atomic>

#include "vertex.h"
#include "bounding_box.h"

class MeshGeometry {
public:

	virtual void generateGeometry() {}

	std::atomic_bool geometry_created = false;
	virtual void createGeometry();

	std::vector<GLuint> indices;
	std::vector<Vertex> vertices;

	GLuint VAO, VBO, EBO;
	BoundingBox boundingBox;
};