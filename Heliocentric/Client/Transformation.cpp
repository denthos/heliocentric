#include "Transformation.h"

Transformation::Transformation() : M(glm::mat4(1.0f)) {
	
}

Transformation::Transformation(glm::mat4 M) : M(M) {

}

void Transformation::setTransformation(glm::mat4 M) {
	this->M = M;
}

glm::mat4 Transformation::getTransformation() {
	return M;
}

void Transformation::draw(GLuint shader, glm::mat4 C) {
	Group::draw(shader, C * M);
}