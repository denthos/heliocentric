#pragma once
#include <unordered_map>
#include <vector>
#include <typeinfo>
#include <typeindex>

/* Forward declaration is necessary so compiler knows about GameObject. We cannot
#include "game_object.h" since it #includes "player.h" */
class GameObject;

class Player {
public:
	Player(std::string player_name, int player_ID);
	std::string get_name();
	void set_name(std::string new_name);
	int get_player_ID();
	void acquire_object(GameObject* object);
	void print();
	std::ostream& operator<<(std::ostream&);

private:
	std::unordered_map<std::type_index, std::vector<GameObject*>*> owned_objects;
	std::string name;
	int ID;
	//list<Resource> recourses;
};
