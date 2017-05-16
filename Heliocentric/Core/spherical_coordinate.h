#pragma once

#include <glm/glm.hpp>

class SphericalCoordinate {
private:
	float theta_radians;
	float phi_radians;
	float theta_degrees;
	float phi_degrees;

public:
	/**
	Create spherical coordinates
	@param theta Degrees from the Y axis (rotating from the positive Y axis to negative Y axis, towards the positive Z axis)
	@param phi Degrees from Z axis (rotating from positive Z axis to negative Z axis, towards the positive Y axis)
	*/
	SphericalCoordinate(float theta, float phi);
	float getThetaRadians() const;
	float getPhiRadians() const;
	float getThetaDegrees() const;
	float getPhiDegrees() const;

	glm::vec3 toCartesian(glm::vec3 origin, float radius) const;
	glm::mat4 getRotationMatrix(glm::mat4 start = glm::mat4(1.0f)) const;
};