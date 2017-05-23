#include "sphere_model.h"

#include "sphere_mesh.h"

#define SPHERE_MESH_RADIUS 1.0f

SphereModel::SphereModel(const Texture* tex) {
	meshes.push_back(SphereMesh(SphereMeshGeometry::getInstance()));
	meshes[0].setTexture(tex);
	calculateBoundingBox();
}

bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1)
{
	float discr = b * b - 4.0f * a * c;
	if (discr < 0) return false;
	else if (discr == 0) x0 = x1 = -0.5f * b / a;
	else {
		float q = (b > 0.0f) ?
			-0.5f * (b + sqrt(discr)) :
			-0.5f * (b - sqrt(discr));
		x0 = q / a;
		x1 = c / q;
	}
	if (x0 > x1) std::swap(x0, x1);

	return true;
}

bool SphereModel::intersect(const Ray & ray, Collision & collision) const {
	// intersect with sphere
	float t, t0, t1; // solutions for t if the ray intersects 
					  // analytic solution
	glm::vec3 L = ray.origin;
	float a = glm::dot(ray.direction, ray.direction);
	float b = 2.0f * glm::dot(ray.direction, L);
	float c = glm::dot(L, L) - SPHERE_MESH_RADIUS;
	if (!solveQuadratic(a, b, c, t0, t1)) return false;
	if (t0 > t1) std::swap(t0, t1);

	if (t0 < 0.0f) {
		t0 = t1; // if t0 is negative, let's use t1 instead 
		if (t0 < 0.0f) return false; // both t0 and t1 are negative 
	}

	t = t0;

	glm::vec3 hitPosition = ray.origin + (t * ray.direction);
	float hitDistance = glm::distance(hitPosition, ray.origin);
	if (hitDistance < collision.distance) {
		collision.position = hitPosition;
		collision.distance = hitDistance;
		return true;
	}

	return false;
}