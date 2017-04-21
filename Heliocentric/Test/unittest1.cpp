#include "stdafx.h"
#include "CppUnitTest.h"

#include <iostream>
#include <string>
#include "player.h"
#include "unit.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			Assert::AreEqual("brandon", "brandon", "Brandon != Brandon!");
		}

     	TEST_METHOD(claim_a_unit_test) {
			Player* sylvia = new Player("Sylvia");
			UID id = 101;
			Unit* battleShip = new Unit(id);

			sylvia->acquire_object(battleShip);
			std::string s("Player (Sylvia) Unit: 1 ids: {101;} ");
			Assert::AreEqual(s, sylvia->to_string());
		}
		
		TEST_METHOD(destroy_a_unit_test) {
			Player sylvia("Sylvia");
			UID id1 = 101;
			UID id2 = 102;
			UID id3 = 103;
			Unit* battleShip1 = new Unit(id1);
			Unit* battleShip2 = new Unit(id2);
			Unit* battleShip3 = new Unit(id3);

			sylvia.acquire_object(battleShip1);
			sylvia.acquire_object(battleShip2);
			sylvia.acquire_object(battleShip3);

			sylvia.add_to_destroy(battleShip1);
			sylvia.pop();

			std::string s("Player (Sylvia) Unit: 2 ids: {102;103;} ");
			Assert::AreEqual(s, sylvia.to_string());
		}

		TEST_METHOD(destroy_multiple_units_test) {
			Player sylvia("Sylvia");
			UID id1 = 101;
			UID id2 = 102;
			UID id3 = 103;
			Unit* battleShip1 = new Unit(id1);
			Unit* battleShip2 = new Unit(id2);
			Unit* battleShip3 = new Unit(id3);

			sylvia.acquire_object(battleShip1);
			sylvia.acquire_object(battleShip2);
			sylvia.acquire_object(battleShip3);

			sylvia.add_to_destroy(battleShip1);
			sylvia.add_to_destroy(battleShip3);

			sylvia.pop();

			std::string s("Player (Sylvia) Unit: 1 ids: {102;} ");
			Assert::AreEqual(s, sylvia.to_string());
		}

		void claim_a_slot_test() {}
	};
}