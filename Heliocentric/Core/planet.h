#pragma once

#include "game_object.h"
#include "slot.h"
#include "planet_types.h"
#include "resources.h"

#include <vector>
#include <string>
#include <iostream>
#include <memory>

class PlanetUpdate;

class Planet : public GameObject {
public:
	friend PlanetUpdate;

	Planet(glm::vec3 position, std::string planet_name, float orbit_speed, float radius, PlanetType type, std::unordered_map<UID, Slot*>, std::unordered_map<Resources::Type, int> resources);
	Planet(UID id, glm::vec3 position, std::string planet_name, float orbit_speed, float radius, PlanetType type, std::unordered_map<UID, Slot*>, std::unordered_map<Resources::Type, int> resources);

	std::unordered_map<UID, Slot*>& get_slots();
	const std::unordered_map<UID, Slot*>& get_slots_const() const;
	void distributeResourcesAmongstSlots();

	void doLogic();
	std::shared_ptr<PlanetUpdate> makeUpdate();

	Slot* get_slot(UID);
	bool check_occupancy() const;

	float get_radius() const;
	PlanetType get_type() const;

	std::string getName() const;

private:
	std::string name;       // planet name
	std::unordered_map<UID, Slot*> slots;  // list of slots available

	PlanetType type;

	float orbit_speed;
	float radius;
	std::shared_ptr<PlanetUpdate> update;

	bool upgraded;          // true if all slots owned by one player

	void initialize();

	/* Resources that the planet has that will be randomly taken by its slots */
	std::unordered_map<Resources::Type, int> resources;
};
