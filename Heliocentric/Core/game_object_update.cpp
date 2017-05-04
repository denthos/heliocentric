#include "game_object_update.h"

GameObjectUpdate::GameObjectUpdate(UID id, float x, float y, float z) : id(id), x(x), y(y), z(z) {

}

void GameObjectUpdate::apply(GameObject* game_object) {
	game_object->update_position(glm::vec3(x, y, z));
}
