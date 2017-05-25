#include "stdafx.h"
#include "CppUnitTest.h"
#include "player.h"
#include "unit.h"
#include "planet.h"
#include "instant_laser_attack.h"
#include "unit_manager.h"

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
			UnitManager manager;
			UID playerID = 123;
			Player sylvia("Sylvia", playerID, PlayerColor::FIRST);
            UID id = 101;
			Attack* att = new InstantLaserAttack();
			Unit* battleShip = new Unit(id, glm::vec3(0.0f, 0.0f, 0.0f), &sylvia, att, &manager, 0, 0);
			Assert::AreEqual(4, (int)sylvia.owned_objects.size());
			sylvia.acquire_object(battleShip);
            Assert::AreEqual(playerID, sylvia.getID());
			Assert::AreEqual(4, (int)sylvia.owned_objects.size());
        }
        
        TEST_METHOD(destroy_a_unit_test) {
			Player sylvia("Sylvia", 123, PlayerColor::FIRST);
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
            Player sylvia("Sylvia", 123, PlayerColor::FIRST);
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
			Planet mars(glm::vec3(0.0f, 0.0f, 0.0f), "Mars", 0.0f, 0.0f, EARTH, std::unordered_map<UID, Slot*>(), std::unordered_map<Resources::Type, int>());

            for (int i = 0; i < 3; ++i) {
				Slot* slot = new Slot(id, &mars, SphericalCoordinate(1.0f, 1.0f));
				mars.get_slots().insert(std::make_pair(id, slot));
                ++id;
            }
            Assert::AreEqual(3, (int) mars.get_slots().size());
            Assert::IsFalse(mars.get_slots().empty());
            Assert::AreEqual(id-1, mars.get_slot(id-1)->getID());
			Assert::IsFalse(mars.get_slot(id - 1)->hasCity());
        }
    };
}
