#include "game_object_update.h"

void GameObjectUpdate::apply(GameObject* game_object) {
	game_object->update_position(glm::vec3(x, y, z));
}
