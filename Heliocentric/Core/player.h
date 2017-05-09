#pragma once
#include "identifiable.h"
#include "resources.h"
#include <unordered_map>
#include <vector>
#include <typeinfo>
#include <typeindex>

#define PLAYER_NAME_MAX_SIZE 16

/* Forward declaration is necessary so compiler knows about GameObject. We cannot
#include "game_object.h" since it #includes "player.h" */
class GameObject;
class PlayerUpdate;

class Player : public Identifiable {
public:
	friend PlayerUpdate;
	Player(std::string player_name);
	Player(std::string player_name, UID id);

	std::string get_name();
	void set_name(std::string new_name);
	void acquire_object(GameObject* object);
	void add_to_destroy(GameObject* object);         // Add a game object to destroy
	void pop();                               // Pop all objects queued for destroy

	float get_resource_amount(Resources::ResourceType);

	std::unordered_map<unsigned int, GameObject*> get_units();   // return list of owned units
	GameObject* get_unit(UID id);
	std::unordered_map<std::type_index, std::unordered_map<unsigned int, GameObject*>> owned_objects;

private:
	std::string name;
	std::vector<GameObject*> objects_to_destroy;
	std::unordered_map<Resources::ResourceType, float> owned_resources; // Stores the amount of each type of resources the player owns

	void initialize(); // Called by both constructors to initialize player.
};
