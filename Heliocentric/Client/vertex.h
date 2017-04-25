#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>



class Vertex {
public:
	glm::vec3 pos; //<x,y,z> position
	glm::vec3 norm; //normal
	glm::vec2 tex_coords; //<u,v> texture coordinates

};


