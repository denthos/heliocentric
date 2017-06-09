#include "unit_update.h"
#include "logging.h"

UnitUpdate::UnitUpdate(UID id, float x, float y, float z) : GameObjectUpdate::GameObjectUpdate(id, x, y, z), health (0), attacking(false) {}

UnitUpdate::UnitUpdate(UID id, int heal, float x, float y, float z) : GameObjectUpdate::GameObjectUpdate(id, x, y, z), health(heal), attacking(false) {}

void UnitUpdate::apply(GameObject* obj) {
	GameObjectUpdate::apply(obj);
	Unit* unit = dynamic_cast<Unit*>(obj);
	if (!unit) {
		LOG_ERR("Object past through unit update is not type unit.");
		return;
	}
	unit->set_health(this->health);
	unit->set_orientation(glm::vec3(orientation_x, orientation_y, orientation_z));
	unit->client_setAttacking(attacking);
}
/*
void UnitUpdate::apply(Unit* obj) {
	GameObjectUpdate::apply(obj);
	Unit* unit = reinterpret_cast<Unit*>(obj);
	glm::vec3 pos = obj->get_position();
}*/
