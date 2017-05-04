#include "planet_update.h"

PlanetUpdate::PlanetUpdate(UID id, float x, float y, float z) : GameObjectUpdate::GameObjectUpdate(id, x, y, z) {

}

void PlanetUpdate::apply(GameObject* obj) {
	GameObjectUpdate::apply(obj);

	Planet* planet = reinterpret_cast<Planet*>(obj);
}