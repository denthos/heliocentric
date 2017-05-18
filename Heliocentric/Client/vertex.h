#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Vertex {
public:
	glm::vec3 pos; //<x,y,z> position
	glm::vec3 norm; //normal
	glm::vec2 tex_coords; //<u,v> texture coordinates

};


