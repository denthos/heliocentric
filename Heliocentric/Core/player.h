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
	Player(std::string player_name, int player_ID);
	std::string get_name();
	void set_name(std::string new_name);
	int get_player_ID();
	void acquire_object(GameObject* object);
	void print();
	void swap_data(GameObject*);              // swap an object in any vector under owned_objects with its last element
	void add_to_destory(GameObject*);         // Add a game object to destroy
	void pop();                               // Pop all objects queued for destroy

private:
	std::unordered_map<std::type_index, std::vector<GameObject*>*> owned_objects;
	std::unordered_map<std::type_index, std::string> type_names;
	std::string name;
	std::vector<GameObject*> objects_to_destroy;

	int ID;
	//list<Resource> recourses;
};
