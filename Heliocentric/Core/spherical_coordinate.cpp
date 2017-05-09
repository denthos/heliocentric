#include "spherical_coordinate.h"

SphericalCoordinate::SphericalCoordinate(float theta, float phi) :
	theta(theta), phi(phi) {}


float SphericalCoordinate::getTheta() const {
	return theta;
}

float SphericalCoordinate::getPhi() const {
	return phi;
}

glm::vec3 SphericalCoordinate::toCartesian(glm::vec3 origin, float radius) const {
	float x = radius * sinf(theta) * cosf(phi);
	float y = radius * sinf(theta) * sinf(phi);
	float z = radius * cosf(theta);

	return origin + glm::vec3(x, y, z);
}