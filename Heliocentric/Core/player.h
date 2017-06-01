#pragma once
#include "identifiable.h"
#include "resources.h"
#include "trade_deal.h"
#include "tech_tree.h"
#include "player_color.h"
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
class PlayerScoreUpdate;
class PlayerManager;

class Player : public Identifiable {
public:
	friend PlayerScoreUpdate;
	friend PlayerUpdate;
	friend NewPlayerInfoUpdate;
	Player(PlayerManager* player_manager, std::string player_name, PlayerColor::Color color);
	Player(std::string player_name, UID id, PlayerColor::Color color);

	void doLogic(); // Perform all logic that is done every server tick.

	std::string get_name() const;
	void set_name(std::string new_name);

	int get_player_score() const;
	PlayerColor::Color getColor() const;
	void setColor(PlayerColor::Color);

	void increase_player_score(int);
	void decrease_player_score(int);

	float get_research_points();

	void acquire_object(GameObject* object);
	void add_to_destroy(GameObject* object);         // Add a game object to destroy
	void pop();                               // Pop all objects queued for destroy

	const ResourceCollection& getResources() const;
	int get_resource_amount(Resources::Type);
	int get_resource_amount(int);
	Resources::Type get_resource_type(int);
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
	PlayerManager* manager;
	std::string name;
	int player_score;
	float research_points; // research points accumulated every tick
  
	TechTree tech_tree;
	PlayerColor::Color color;

	std::shared_ptr<PlayerScoreUpdate> score_update;
	void send_update_to_manager(std::shared_ptr<PlayerScoreUpdate> update);
  
	std::vector<GameObject*> objects_to_destroy;
	std::unordered_map<Resources::Type, int> owned_resources; // Stores the amount of each type of resources the player owns
	// std::queue<std::shared_ptr<TradeDeal>> active_trade_deals; // All active trade deals that involves this player, not implemented yet
	std::unordered_map<UID, std::shared_ptr<TradeDeal>> pending_trade_deals; // All pending trade deals waiting to be accepted or declined

	void initialize(); // Called by both constructors to initialize player.
};
