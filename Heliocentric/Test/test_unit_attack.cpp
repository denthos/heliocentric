#include "stdafx.h"
#include "CppUnitTest.h"
#include "unit.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{

	class TestUnit : public Unit {

	public: 
		TestUnit(UID id, glm::vec3 position) : Unit(id, position, NULL, 1, 1, 200, 100) {
			this->movementSpeedMax = 200;
		}

		bool is_in_attack_mode() {
			return this->currentCommand == Unit::UNIT_ATTACK;
		}

		bool is_in_idle_mode() {
			return this->currentCommand == Unit::UNIT_IDLE;
		}
	};

	TEST_CLASS(UnitAttackTest)
	{
	public:

		/**
		Test that setting target puts unit in attack mode.
		*/
		TEST_METHOD(set_combat_target_test) {
			/* Setup */
			TestUnit* unit_1 = new TestUnit(100, glm::vec3(2.0f));
			TestUnit* unit_2 = new TestUnit(101, glm::vec3(3.0f));

			/* Combat */
			unit_1->set_combat_target(unit_2);
			unit_1->set_command(Unit::UNIT_ATTACK);
			Assert::IsTrue(unit_1->is_in_attack_mode());
			Assert::IsTrue(unit_2->is_in_idle_mode());
		}

		/**
		Test that basic combat between units.
		*/
		TEST_METHOD(basic_unit_attack_test) {
			/* Setup */
			Player player_1("Player 1", 102);
			Player player_2("Player 2", 203);
			TestUnit* unit_1 = new TestUnit(100, glm::vec3(2.0f));
			TestUnit* unit_2 = new TestUnit(101, glm::vec3(3.0f));
			player_1.acquire_object(unit_1);
			player_2.acquire_object(unit_2);

			/* Combat */
			unit_1->set_combat_target(unit_2);
			unit_1->set_command(Unit::UNIT_ATTACK);
			unit_1->update();
			Assert::AreEqual(unit_2->get_health(), 50);
			Assert::AreEqual(unit_1->get_health(), 100);
		}

		/**
		Test that attacking unit gets closer to target if it's out of range.
		*/
		TEST_METHOD(out_of_range_test) {
			/* Setup */
			Player player_1("Player 1", 102);
			Player player_2("Player 2", 203);
			TestUnit* unit_1 = new TestUnit(100, glm::vec3(0.0f));
			TestUnit* unit_2 = new TestUnit(101, glm::vec3(250.0f));
			player_1.acquire_object(unit_1);
			player_2.acquire_object(unit_2);

			Assert::IsTrue(glm::distance(unit_1->get_position(), unit_2->get_position()) >= 250.0f);

			/* Combat */
			unit_1->set_combat_target(unit_2);
			unit_1->set_command(Unit::UNIT_ATTACK);
			unit_1->update();

			// Unit got closer
			Assert::IsTrue(glm::distance(unit_1->get_position(), unit_2->get_position()) < 250.0f);
			Assert::IsTrue(unit_1->is_in_attack_mode());
			Assert::AreEqual(unit_1->get_health(), 100);
			Assert::AreEqual(unit_2->get_health(), 100);
		}


		/**
		Test that a unit whose health falls below zero is removed from the game
		TODO: Player::get_unit(id) does not work with subclass of Unit
		*/
		TEST_METHOD(unit_death_test) {
			/* Setup */
			Player player_1("Player 1", 102);
			Player player_2("Player 2", 203);
			TestUnit* unit_1 = new TestUnit(100, glm::vec3(0.0f));
			TestUnit* unit_2 = new TestUnit(101, glm::vec3(0.0f));

			player_1.acquire_object(unit_1);
			player_2.acquire_object(unit_2);
			Assert::IsNotNull(unit_2->get_player());

			/* Combat */
			unit_1->set_combat_target(unit_2);
			unit_1->set_command(Unit::UNIT_ATTACK);

			// Kill unit_2 : should take two hits
			unit_1->update();
			unit_1->update();

			// Check  dthat unit_2 is dead and unit_1 is still alive
			Assert::IsTrue(unit_1->is_in_idle_mode());
			Assert::IsNull(unit_2->get_player());
		}


	};
}