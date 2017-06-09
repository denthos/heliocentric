#pragma once

#define CITY_NAME_SIZE 16

#include "attackable_game_object.h"
#include "slot.h"
#include "player.h"
#include "builder.h"

class CityUpdate;
class CityManager;

class City : public AttackableGameObject, public Builder {
public:
	friend CityUpdate;

	City(Player* owner, Attack* attack, CityManager* manager, int def, int heal, int pr, int pop, Slot* assigned_slot, std::string name);
	City(UID id, Player* owner, Attack* attack, CityManager* manager, int def, int heal, int pr, int pop, Slot* assigned_slot, std::string name);

	int get_population() const;
	void set_population(int new_pop);
	int get_research_points() const;
	void set_research_points(int new_rp);
	Slot* get_slot();
	glm::vec3 get_position() const;

	/**
	Perform update.
	*/
	std::shared_ptr<CityUpdate> make_update();

	void extractResourcesFromSlotAndCreateUpdates(std::vector<std::shared_ptr<PlayerUpdate>>&, std::vector<std::shared_ptr<SlotUpdate>>&);
	std::string getName() const;

private:
	std::string name;
	int population;
	int research_points;
	Slot* slot;

	void initialize();

protected:
	// TODO: Override these combat hooks
	virtual void handle_out_of_range(std::shared_ptr<AttackableGameObject> opponent);
	virtual void handle_defeat(std::shared_ptr<AttackableGameObject> opponent);
	virtual void handle_victory(std::shared_ptr<AttackableGameObject> opponent);
	virtual void handle_counter(std::shared_ptr<AttackableGameObject> opponent);
	void send_update_to_manager(std::shared_ptr<CityUpdate> update);

	/* Override for UnitSpawner */
	void spawnCompleteHandler(Buildable* type, Builder::ProductionType buildType);
	
	std::shared_ptr<CityUpdate> update;
	std::shared_ptr<AttackableGameObject> target;
	CityManager* manager;
};
