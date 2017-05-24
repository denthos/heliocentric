#pragma once
#include "identifiable.h"
#include "resources.h"
#include "trade_deal.h"
#include "tech_tree.h"
#include <queue>
#include <unordered_map>
#include <vector>
#include <typeinfo>
#include <typeindex>

#define PLAYER_NAME_MAX_SIZE 16

/* Forward declaration is necessary so compiler knows about GameObject. We cannot
#include "game_object.h" since it #includes "player.h" */
class GameObject;
class PlayerUpdate;
class NewPlayerInfoUpdate;

class Player : public Identifiable {
public:
	friend PlayerUpdate;
	friend NewPlayerInfoUpdate;
	Player(std::string player_name);
	Player(std::string player_name, UID id);

	std::string get_name();
	void set_name(std::string new_name);

	float get_player_score();
	void increase_player_score(float);
	void decrease_player_score(float);

	void acquire_object(GameObject* object);
	void add_to_destroy(GameObject* object);         // Add a game object to destroy
	void pop();                               // Pop all objects queued for destroy

	int get_resource_amount(Resources::Type);
	void change_resource_amount(Resources::Type, int);

	template <typename T>
	std::unordered_map<UID, GameObject*> getOwnedObjects() {
		auto owned_objs_of_type = owned_objects.find(typeid(T));
		if (owned_objs_of_type == owned_objects.end()) {
			return std::unordered_map<UID, GameObject*>();
		}

		return owned_objs_of_type->second;
	}

	std::unordered_map<UID, GameObject*> get_units();   // return list of owned units
	GameObject* get_unit(UID id);

	void receive_trade_deal(std::shared_ptr<TradeDeal>); // Add newly received trade deal to pending list
	UID trade_deal_accept(); // TEMPORARY: Accept the first trade deal in pending deals map
	UID trade_deal_decline(); // TEMPORARY: Decline the first trade deal in pending deals map
	void trade_deal_accept(UID); // Accept the specified trade deal and move it to active trade deals if applies
	void trade_deal_decline(UID); // Decline the specified trade deal

	std::shared_ptr<TradeDeal> get_trade_deal(); // TEMPORARY: Return the first trade deal in map
	std::shared_ptr<TradeDeal> get_trade_deal(UID); // Return a trade deal by UID

	std::unordered_map<std::type_index, std::unordered_map<UID, GameObject*>> owned_objects; // TODO: move to private

private:
	std::string name;
	float player_score;
	TechTree tech_tree;
  
	std::vector<GameObject*> objects_to_destroy;
	std::unordered_map<Resources::Type, int> owned_resources; // Stores the amount of each type of resources the player owns
	// std::queue<std::shared_ptr<TradeDeal>> active_trade_deals; // All active trade deals that involves this player, not implemented yet
	std::unordered_map<UID, std::shared_ptr<TradeDeal>> pending_trade_deals; // All pending trade deals waiting to be accepted or declined

	void initialize(); // Called by both constructors to initialize player.
};
