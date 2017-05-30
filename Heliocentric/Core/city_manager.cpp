#include "city_manager.h"
#include "instant_laser_attack.h"
#include "unit_spawner.h"
#include "planet.h"

void CityManager::handleUnitSpawningComplete(UnitType* type, City* city) {
	glm::vec3 pos = city->get_position() + glm::vec3(city->get_slot()->get_spherical_position().getRotationMatrix() * glm::vec4(0.0f, city->get_slot()->getPlanet()->get_radius() * 1.15f, 0.0f, 0.0f));
	unit_creation_updates.insert(unit_manager->add_unit(pos, type, city->get_player()));
}

CityManager::CityManager(UnitManager* unit_manager) : unit_manager(unit_manager) {}

void CityManager::doLogic() {
	this->city_updates.clear();
	this->spawner_updates.clear();
	this->unit_creation_updates.clear();

	/* Now we are going to look at all cities and see if we can update their spawners */
	for (auto& city_pair : get_cities()) {
		City* city = city_pair.second.get();

		if (city->progressSpawnAndCreateUpdate()) {
			this->spawner_updates.insert(city->getSpawnUpdate());
		}
	}
}

City* CityManager::get_city(UID uid) const {
	auto& it = cities.find(uid);
	return (it == cities.end()) ? nullptr : it->second.get();
}

std::unordered_map < UID, std::unique_ptr<City>>& CityManager::get_cities() {
	return this->cities;
}

std::shared_ptr<CityCreationUpdate> CityManager::add_city(Player* player, Slot* slot, std::string name) {
	// TODO: Create the city from the player's current technologies
	std::unique_ptr<City> new_city = std::make_unique<City>(player, new InstantLaserAttack(), this, 100, 100, 0, 0, slot, name);
	slot->attachCity(new_city.get());
	player->acquire_object(new_city.get());
	auto city_creation_update = std::make_shared<CityCreationUpdate>(player->getID(), slot->getID(), new_city->getID(), new_city->getName());
	LOG_DEBUG("Created city with UID <", new_city->getID(), ">");
	cities.insert(std::make_pair(new_city->getID(), std::move(new_city)));
	return city_creation_update;
}


std::unordered_set<std::shared_ptr<CityUpdate>>& CityManager::get_updates() {
	return this->city_updates;
}

void CityManager::register_update(std::shared_ptr<CityUpdate>& update) {
	this->city_updates.insert(update);
	// TODO: Destroy units based on client command.
	if (update->health <= 0) {
		LOG_DEBUG("Removing city with UID <", update->id, ">");
		this->cities.erase(update->id);
	}

}

const std::unordered_set<std::shared_ptr<UnitSpawnerUpdate>>& CityManager::getSpawnerUpdates() const {
	return this->spawner_updates;
}


const std::unordered_set<std::shared_ptr<UnitCreationUpdate>>& CityManager::getCreationUpdates() const {
	return this->unit_creation_updates;
}

std::shared_ptr<UnitSpawnerUpdate> CityManager::spawnUnit(std::shared_ptr<PlayerCommand> command) {
	auto& city_pair = this->cities.find(command->initiator);
	if (city_pair == this->cities.end()) {
		LOG_ERR("Invalid city Id sent to server");
	}

	return city_pair->second->spawnUnit(command->createUnitType);
}
