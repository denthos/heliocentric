#include "unit_update.h"

UnitUpdate::UnitUpdate(UID id, float x, float y, float z) : GameObjectUpdate::GameObjectUpdate(id, x, y, z) {

}

void UnitUpdate::apply(GameObject* obj) {
	GameObjectUpdate::apply(obj);
	Unit* unit = reinterpret_cast<Unit*>(obj);
	glm::vec3 pos = obj->get_position();
}