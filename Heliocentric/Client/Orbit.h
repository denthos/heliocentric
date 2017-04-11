#ifndef ORBIT_H_
#define ORBIT_H_

#include "Group.h"

class Orbit : public Group {
public:
	Orbit(float distance, float speed, glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f));
	void setDistance(float);
	void setSpeed(float);
	void setAxis(glm::vec3);
	float getDistance();
	float getSpeed();
	glm::vec3 getAxis();
	glm::mat4 getTransformation();
	virtual void draw(GLuint, glm::mat4);
	virtual void update();
protected:
	float distance, speed;
	glm::vec3 axis;
	glm::mat4 M;
};

#endif