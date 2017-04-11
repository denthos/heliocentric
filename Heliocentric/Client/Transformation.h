#ifndef TRANSFORMATION_H_
#define TRANSFORMATION_H_

#include "Group.h"

class Transformation : public Group {
public:
	Transformation();
	Transformation(glm::mat4);
	void setTransformation(glm::mat4);
	glm::mat4 getTransformation();
	virtual void draw(GLuint, glm::mat4);
protected:
	glm::mat4 M;
};

#endif