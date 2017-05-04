#include "unit_creation_update.h"

UnitCreationUpdate::UnitCreationUpdate(UID id, float x, float y, float z,
	UID player_id, int att, int def, int range, int health) :
	GameObjectUpdate(id, x, y, z), player_id(player_id),
	att(att), def(def), range(range), health(health) {

}

void UnitCreationUpdate::apply(GameObject* obj) {
	GameObjectUpdate::apply(obj);
	Unit* unit = reinterpret_cast<Unit*>(obj);
}