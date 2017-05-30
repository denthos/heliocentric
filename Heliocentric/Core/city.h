#pragma once

#define CITY_NAME_SIZE 16

#include "attackable_game_object.h"
#include "slot.h"
#include "player.h"
#include "unit_spawner.h"

class CityUpdate;
class CityManager;

class City : public AttackableGameObject, public UnitSpawner {
public:
	friend CityUpdate;

	City(Player* owner, Attack* attack, CityManager* manager, int def, int heal, int pr, int pop, Slot* assigned_slot, std::string name);
	City(UID id, Player* owner, Attack* attack, CityManager* manager, int def, int heal, int pr, int pop, Slot* assigned_slot, std::string name);

	int get_population();
	void set_population(int new_pop);
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
	int production;
	int population;
	Slot* slot;

	void initialize();

protected:
	// TODO: Override these combat hooks
	virtual void handle_out_of_range(AttackableGameObject* opponent);
	virtual void handle_defeat(AttackableGameObject* opponent);
	virtual void handle_victory(AttackableGameObject* opponent);
	virtual void handle_counter(AttackableGameObject* opponent);
	void send_update_to_manager(std::shared_ptr<CityUpdate>& update);

	/* Override for UnitSpawner */
	void spawnCompleteHandler(UnitType* type);
	
	std::shared_ptr<CityUpdate> update;
	AttackableGameObject* target = nullptr;
	CityManager* manager;
};
