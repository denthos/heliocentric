#include "lib.h"
#include "ini_parser.h"
#include "glm/ext.hpp"
#include <glm\vec3.hpp>

void config_test() {
	/* Load config file */
	Lib::INIParser config = Lib::INIParser::getInstance("ConfigTemplate.ini");

	/* Retrieve an int from the config file. */
	int value = 600;  // Set 600 as the default.
	value = config.get<int>("ScreenWidth");
	assert(value == 800);
}

int main() {
	config_test();
}
