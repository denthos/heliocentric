#include "unit_update.h"
#include "logging.h"
void UnitUpdate::apply(GameObject* obj) {
	GameObjectUpdate::apply(obj);
	Unit* unit = reinterpret_cast<Unit*>(obj);
	glm::vec3 pos = obj->get_position();
	Lib::LOG_DEBUG("Position is " + std::to_string(pos.x) + " " + std::to_string(pos.y) + " " + std::to_string(pos.z));
}