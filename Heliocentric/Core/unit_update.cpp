#include "unit_update.h"
#include "logging.h"
void UnitUpdate::apply(GameObject* obj) {
	GameObjectUpdate::apply(obj);
	Unit* unit = reinterpret_cast<Unit*>(obj);
	Lib::LOG_DEBUG("Position is " + std::to_string(unit->get_position().x) + " " + std::to_string(unit->get_position().y) + " " + std::to_string(unit->get_position().z));
}