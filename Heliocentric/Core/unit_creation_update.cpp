#include "unit_creation_update.h"

UnitCreationUpdate::UnitCreationUpdate(UID id, float x, float y, float z,
	UID player_id, int def, int health, UnitType::TypeIdentifier type) :
	GameObjectUpdate(id, x, y, z), player_id(player_id),
	def(def), health(health), type(type) {

}

void UnitCreationUpdate::apply(GameObject* obj) {
	GameObjectUpdate::apply(obj);
	Unit* unit = reinterpret_cast<Unit*>(obj);
}