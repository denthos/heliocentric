#include "stdafx.h"
#include "CppUnitTest.h"
#include "player.h"
#include "unit.h"
#include "planet.h"

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
			UID playerID = 123;
            Player sylvia("Sylvia", playerID);
            UID id = 101;
			Slot* slot = new Slot(id, glm::vec3(0.0f, 1.0f, id));
            Unit* battleShip = new Unit(id, glm::vec3(0.0f, 0.0f, 0.0f), &sylvia, 25, 1, 0, 100);
			Assert::AreEqual(4, (int)sylvia.owned_objects.size());
            //sylvia.acquire_object(slot);
			sylvia.acquire_object(battleShip);
            Assert::AreEqual(playerID, sylvia.getID());
			//Assert::AreEqual(playerID , battleShip->get_player()->getID());
			Assert::AreEqual(4, (int)sylvia.owned_objects.size());
			//Assert::AreEqual(1, (int) sylvia.get_units().size());
            //Assert::AreEqual(id, sylvia.get_unit(id)->getID());
        }
        
        TEST_METHOD(destroy_a_unit_test) {
            Player sylvia("Sylvia", 123);
            UID id1 = 101;
            UID id2 = 102;
            UID id3 = 103;
            Unit* battleShip1 = new Unit(id1, glm::vec3(0.0f, 0.0f, 0.0f), &sylvia, 0, 0, 0, 0);
            Unit* battleShip2 = new Unit(id2, glm::vec3(0.0f, 0.0f, 0.0f), &sylvia, 0, 0, 0, 0);
            Unit* battleShip3 = new Unit(id3, glm::vec3(0.0f, 0.0f, 0.0f), &sylvia, 0, 0, 0, 0);

            sylvia.acquire_object(battleShip1);
            sylvia.acquire_object(battleShip2);
            sylvia.acquire_object(battleShip3);

            sylvia.add_to_destroy(battleShip1);
            sylvia.pop();

            Assert::AreEqual(2, (int)sylvia.get_units().size());
            Assert::AreEqual(id2, sylvia.get_unit(id2)->getID());
            Assert::AreEqual(id3, sylvia.get_unit(id3)->getID());
            Assert::IsNull(sylvia.get_unit(id1));
        }

        TEST_METHOD(destroy_multiple_units_test) {
            Player sylvia("Sylvia", 123);
            UID id1 = 101;
            UID id2 = 102;
            UID id3 = 103;
            Unit* battleShip1 = new Unit(id1, glm::vec3(0.0f, 0.0f, 0.0f), &sylvia, 0, 0, 0, 0);
            Unit* battleShip2 = new Unit(id2, glm::vec3(0.0f, 0.0f, 0.0f), &sylvia, 0, 0, 0, 0);
            Unit* battleShip3 = new Unit(id3, glm::vec3(0.0f, 0.0f, 0.0f), &sylvia, 0, 0, 0, 0);

            sylvia.acquire_object(battleShip1);
            sylvia.acquire_object(battleShip2);
            sylvia.acquire_object(battleShip3);

            sylvia.add_to_destroy(battleShip1);
            sylvia.add_to_destroy(battleShip3);

            sylvia.pop();

            Assert::AreEqual(1, (int) sylvia.get_units().size());
            Assert::IsNull(sylvia.get_unit(id1));
            Assert::AreEqual(id2, sylvia.get_unit(id2)->getID());
            Assert::IsNull(sylvia.get_unit(id3));
        }

        TEST_METHOD(create_planet_test) {
            UID id = 100;
            std::unordered_map<UID, Slot*> map;
            for (int i = 0; i < 3; ++i) {
                Slot* slot = new Slot(id, glm::vec3(0.0f, 1.0f, id));
                map.insert(std::pair<UID, Slot*>(id, slot));
                ++id;
            }
			Planet mars(glm::vec3(0.0f, 0.0f, 0.0f), "Mars", 0.0f, 0.0f, map);
            Assert::AreEqual(3, (int) mars.get_slots().size());
            Assert::IsFalse(mars.get_slots().empty());
            Assert::AreEqual(id-1, mars.get_slot(id-1)->getID());
            Assert::IsNull(mars.get_slot(id-1)->get_player());
        }
    };
}
