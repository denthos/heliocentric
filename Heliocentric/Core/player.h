#pragma once
#include "identifiable.h"
#include <unordered_map>
#include <vector>
#include <typeinfo>
#include <typeindex>

/* Forward declaration is necessary so compiler knows about GameObject. We cannot
#include "game_object.h" since it #includes "player.h" */
class GameObject;
struct PlayerUpdate;

class Player : public Identifiable {
public:
	friend PlayerUpdate;
	Player(std::string player_name);
	std::string get_name();
	void set_name(std::string new_name);
	void acquire_object(GameObject* object);
	std::string to_string();
	void add_to_destroy(GameObject*);         // Add a game object to destroy
	void pop();                               // Pop all objects queued for destroy

private:
	std::unordered_map<std::type_index, std::unordered_map<unsigned int, GameObject*>> owned_objects;
	std::unordered_map<std::type_index, std::string> type_names;
	std::string name;
	std::vector<GameObject*> objects_to_destroy;
	//list<Resource> recourses;
};
