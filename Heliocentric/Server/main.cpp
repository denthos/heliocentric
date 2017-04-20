#include <iostream>
#include <string>
#include "player.h"
#include "unit.h"
//#include "game_channels.h"

void claim_a_unit_test() {
	std::cout << "Claim a single unit test." << std::endl;
	Player sylvia("Sylvia", 1);
	Unit* battleShip = new Unit();

	sylvia.acquire_object(battleShip);
	sylvia.print();
}

void destroy_a_unit_test() {
	std::cout << "Destroy a single unit test." << std::endl;
	Player sylvia("Sylvia", 1);
	Unit* battleShip1 = new Unit();
	Unit* battleShip2 = new Unit();
	Unit* battleShip3 = new Unit();

	sylvia.acquire_object(battleShip1);
	sylvia.acquire_object(battleShip2);
	sylvia.acquire_object(battleShip3);

	sylvia.add_to_destory(battleShip1);
	sylvia.pop();

	sylvia.print();
}

void destroy_multiple_units_test() {
	std::cout << "Destroy multiple units test." << std::endl;
	Player sylvia("Sylvia", 1);
	Unit* battleShip1 = new Unit();
	Unit* battleShip2 = new Unit();
	Unit* battleShip3 = new Unit();

	sylvia.acquire_object(battleShip1);
	sylvia.acquire_object(battleShip2);
	sylvia.acquire_object(battleShip3);

	sylvia.add_to_destory(battleShip1);
	sylvia.add_to_destory(battleShip3);

	sylvia.pop();

	sylvia.print();
}

void claim_a_slot_test() {}

int main() {
	//initializeChannels();

	claim_a_unit_test();
	destroy_a_unit_test();
	destroy_multiple_units_test();
	std::string dummy;
	std::cin >> dummy;
}