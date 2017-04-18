#include "lib.h"

void Lib::assertTrue(bool expression, std::string message) {
	if (!expression) {
		std::cerr << message << std::endl;
	}
}

void Lib::debug(std::string message) {
	std::cerr << message << std::endl;
}

float Lib::distance_vec3(glm::vec3 position1, glm::vec3 position2) {
	glm::vec3 diff = position1 - position2;
	return glm::dot(diff, diff); // calculate magnitude of the diff vector
}