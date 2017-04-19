#pragma once
#include "glm\glm.hpp"

class Transformation {
public:
	Transformation();
	Transformation(glm::mat4);
	void setTransformation(glm::mat4);
	glm::mat4 getTransformation();
	//virtual void Render(Shader, glm::mat4);
protected:
	glm::mat4 M;
};

