#include "unit_update.h"
#include "logging.h"

UnitUpdate::UnitUpdate(UID id, float x, float y, float z) : GameObjectUpdate::GameObjectUpdate(id, x, y, z), health (100) {}

UnitUpdate::UnitUpdate(UID id, int heal, float x, float y, float z) : GameObjectUpdate::GameObjectUpdate(id, x, y, z), health(heal) {}

void UnitUpdate::apply(GameObject* obj) {
	GameObjectUpdate::apply(obj);
	Unit* unit = dynamic_cast<Unit*>(obj);
	if (!unit) {
		LOG_ERR("Object past through unit update is not type unit.");
		return;
	}
	unit->set_health(this->health);
	unit->set_rotation(this->rot_mat);
	unit->set_laser_shooting(this->shoot_laser);
}