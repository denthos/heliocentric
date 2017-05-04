#include "unit_update.h"
#include "logging.h"
void UnitUpdate::apply(GameObject* obj) {
	GameObjectUpdate::apply(obj);
	Unit* unit = reinterpret_cast<Unit*>(obj);
	glm::vec3 pos = obj->get_position();
}