#include "spherical_coordinate.h"
#include <glm/gtc/constants.hpp>
#include <glm/gtx/transform.hpp>

#define toRad(x) (x * (glm::pi<float>() / 180.0f))

SphericalCoordinate::SphericalCoordinate(float theta, float phi) :
	theta_radians(toRad(theta)), phi_radians(toRad(phi)), theta_degrees(theta), phi_degrees(phi) {
}


float SphericalCoordinate::getThetaRadians() const {
	return theta_radians;
}

float SphericalCoordinate::getPhiRadians() const {
	return phi_radians;
}

float SphericalCoordinate::getThetaDegrees() const {
	return theta_degrees;
}

float SphericalCoordinate::getPhiDegrees() const {
	return phi_degrees;
}

glm::vec3 SphericalCoordinate::toCartesian(glm::vec3 origin, float radius) const {
	float x = radius * glm::sin(theta_radians) * glm::cos(phi_radians);
	float y = radius * glm::sin(theta_radians) * glm::sin(phi_radians);
	float z = radius * glm::cos(theta_radians);

	/* OpenGL coords are in a different order than math. */
	return origin + glm::vec3(y, z, x);
}

glm::mat4 SphericalCoordinate::getRotationMatrix(glm::mat4 start) const {
	/* First, rotate on the X axis */
	auto first_rotation = glm::rotate(start, getThetaRadians(), glm::vec3(1.0f, 0.0f, 0.0f));

	/* Then, rotate on the Y axis */
	auto second_rotation = glm::rotate(getPhiRadians(), glm::vec3(0.0f, 1.0f, 0.0f));

	return second_rotation * first_rotation;
}