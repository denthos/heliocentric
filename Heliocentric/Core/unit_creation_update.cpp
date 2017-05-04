#include "unit_creation_update.h"

void UnitCreationUpdate::apply(GameObject* obj) {
	GameObjectUpdate::apply(obj);
	Unit* unit = reinterpret_cast<Unit*>(obj);
}