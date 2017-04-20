#include "lib.h"
#include "ini_parser.h"
#include "player.h"
#include "city.h"
#include "slot.h"
#include "planet.h"
#include "glm/ext.hpp"
#include <glm\vec3.hpp>
#include <iostream>

void city_test_1(Player* player) {
	Slot slot1_for_maya(1, false, glm::vec3(0, 0, 1));
	slot1_for_maya.assign_player(player);
	City maya(5, 0, 100, 1, 1000, &slot1_for_maya);
	glm::vec3 pos = maya.get_slot()->get_position();
	std::cout << glm::to_string(pos) << std::endl;
	maya.print();
}

void config_test() {
	/* Load config file */
	Lib::INIParser config("ConfigTemplate.ini");

	/* Retrieve an int from the config file. */
	int value = 600;  // Set 600 as the default.
	bool exists = config.get_value("ScreenWidth", value);
	assert(exists && value == 800);
}

int main() {
	config_test();

	Player sylvia("Sylvia", 1);
	sylvia.print();

	//city_test_1(&sylvia);
	//Planet mars("Mars", 3);
	//mars.print();

	//std::string dummy;
	//std::cin >> dummy;
}