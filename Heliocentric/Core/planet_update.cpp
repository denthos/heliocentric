#include "planet_update.h"

void PlanetUpdate::apply(GameObject* obj) {
	GameObjectUpdate::apply(obj);

	Planet* planet = reinterpret_cast<Planet*>(obj);
}