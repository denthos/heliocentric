#include "skybox_geometry.h"

void SkyboxMeshGeometry::generateGeometry() {
	indices = { //order of vertices
		0,1,2, //back (direction user is facing initially)
		2,3,0,

		6,5,4,//front (behind user initially)
		4,7,6,

		10,9,8,
		8,11,10, //left

		14,13,12,
		12,15,14, //right

		18,17,16,
		16,19,18, //bottom

		22,23,20,
		20,21,22 //top	
	};

	//coordinates for the corners of a unit cube centered at origin
	glm::vec3 backBottomLH(-0.5f);
	glm::vec3 backBottomRH(0.5f, -0.5f, -0.5f);
	glm::vec3 backTopRH(0.5f, 0.5f, -0.5f);
	glm::vec3 backTopLH(-0.5f, 0.5f, -0.5f);

	glm::vec3 frontBottomLH = backBottomLH * glm::vec3(1.0f, 1.0f, -1.0f);
	glm::vec3 frontBottomRH = backBottomRH * glm::vec3(1.0f, 1.0f, -1.0f);
	glm::vec3 frontTopRH = backTopRH * glm::vec3(1.0f, 1.0f, -1.0f);
	glm::vec3 frontTopLH = backTopLH * glm::vec3(1.0f, 1.0f, -1.0f);

	//construct vertex vector
	Vertex vertex;

	//back face
	vertex.pos = backBottomLH; //bottom left 0
	vertices.push_back(vertex);

	vertex.pos = backBottomRH; //bottom right 1
	vertices.push_back(vertex);

	vertex.pos = backTopRH; //top right 2
	vertices.push_back(vertex);

	vertex.pos = backTopLH; //top left 3
	vertices.push_back(vertex);

	//front face
	vertex.pos = frontBottomLH; //bottom left 4
	vertices.push_back(vertex);

	vertex.pos = frontBottomRH; //bottom right 5
	vertices.push_back(vertex);

	vertex.pos = frontTopRH; //top right 6
	vertices.push_back(vertex);

	vertex.pos = frontTopLH; //top left 7
	vertices.push_back(vertex);

	//left face
	vertex.pos = backBottomLH; //bottom left 8
	vertices.push_back(vertex);

	vertex.pos = frontBottomLH; //bottom right 9
	vertices.push_back(vertex);

	vertex.pos = frontTopLH; //top right 10
	vertices.push_back(vertex);

	vertex.pos = backTopLH; //top left 11
	vertices.push_back(vertex);

	//right face
	vertex.pos = frontBottomRH; //bottom left 12
	vertices.push_back(vertex);

	vertex.pos = backBottomRH; //bottom right 13
	vertices.push_back(vertex);

	vertex.pos = backTopRH; //top right 14
	vertices.push_back(vertex);

	vertex.pos = frontTopRH; //top left 15
	vertices.push_back(vertex);

	//bottom face
	vertex.pos = backBottomLH; //bottom left 16
	vertices.push_back(vertex);

	vertex.pos = backBottomRH; //bottom right 17
	vertices.push_back(vertex);

	vertex.pos = frontBottomRH; //top right 18
	vertices.push_back(vertex);

	vertex.pos = frontBottomLH; //top left 19
	vertices.push_back(vertex);

	//top face
	vertex.pos = backTopLH; //bottom left 20
	vertices.push_back(vertex);

	vertex.pos = backTopRH; //bottom right 21
	vertices.push_back(vertex);

	vertex.pos = frontTopRH; //top right 22
	vertices.push_back(vertex);

	vertex.pos = frontTopLH; //top left 23
	vertices.push_back(vertex);
}