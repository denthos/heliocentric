#include "city_manager.h"
#include "instant_laser_attack.h"


CityManager::CityManager() {}

void CityManager::doLogic() {
	this->city_updates.clear();
}

std::unordered_map < UID, std::unique_ptr<City>>& CityManager::get_cities() {
	return this->cities;
}

std::shared_ptr<CityCreationUpdate> CityManager::add_city(Player* player, Slot* slot, std::string name) {
	// TODO: Create the city from the player's current technologies
	std::unique_ptr<City> new_city = std::make_unique<City>(player, new InstantLaserAttack(), this, 0, 0, 0, 0, slot, name);
	slot->attachCity(new_city.get());
	player->acquire_object(new_city.get());
	auto city_creation_update = std::make_shared<CityCreationUpdate>(player->getID(), slot->getID(), new_city->getID(), new_city->getName());
	cities.insert(std::make_pair(new_city->getID(), std::move(new_city)));
	return city_creation_update;
}


std::unordered_set<std::shared_ptr<CityUpdate>>& CityManager::get_updates() {
	return this->city_updates;
}

void CityManager::register_update(std::shared_ptr<CityUpdate>& update) {
	this->city_updates.insert(update);
}