#pragma once

#include <glm/glm.hpp>

class SphericalCoordinate {
private:
	float theta;
	float phi;

public:
	SphericalCoordinate(float theta, float phi);
	float getTheta() const;
	float getPhi() const;

	glm::vec3 toCartesian(glm::vec3 origin, float radius) const;
};