#include "player.h"
#include "city.h"
#include "slot.h"
#include "planet.h"
//#include "unit.h"
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

int main() {
	Player sylvia("Sylvia", 1);
	sylvia.print();

	city_test_1(&sylvia);
	//Planet mars("Mars", 3);
	//mars.print();

	//Unit fighter(player1, pos);
	//std::cout << glm::to_string(fighter.get_position()) << std::endl;
	std::string dummy;
	std::cin >> dummy;
}