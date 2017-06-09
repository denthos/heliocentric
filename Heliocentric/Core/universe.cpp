#include "universe.h"
#include "slot.h"

Universe::Universe() {
	/* No concept of slots yet -- so let's just share slots amongst planets*/
	std::unordered_map<UID, Slot*> slots;

	/* Generate standard planets for this universe */
	this->planets.insert(createPlanet(glm::vec3(0.0f, 0.0f, 0.0f), "Sun", 0.0f, 60.0f * 2.5f, SUN, slots,
		std::unordered_map<Resources::Type, int> {
	}
	));
	this->planets.insert(createPlanet(glm::vec3(200.0f * 5.0f, 0.0f, 0.0f), "Mercury", 0.5f / 10.0f, 20.0f * 2.5f, MERCURY, slots,
		std::unordered_map<Resources::Type, int> {
			{ Resources::ALUMINUM, 1000 },
			{ Resources::GOLD, 1000 },
			{ Resources::URANIUM, 3000 },
			{ Resources::NANOMATERIAL, 3000 }
	}
	));
	this->planets.insert(createPlanet(glm::vec3(440.0f * 5.0f, 0.0f, 0.0f), "Venus", 0.3f / 10.0f, 24.0f * 2.5f, VENUS, slots,
		std::unordered_map<Resources::Type, int> {
			{ Resources::ALUMINUM, 1000 },
			{ Resources::GOLD, 2000 },
			{ Resources::TITANIUM, 3000 },
			{ Resources::NANOMATERIAL, 6000 }
	}
	));
	this->planets.insert(createPlanet(glm::vec3(700.0f * 5.0f, 0.0f, 0.0f), "Earth", 0.2f / 10.0f, 28.0f * 2.5f, EARTH, slots,
		std::unordered_map<Resources::Type, int> {
			{ Resources::ALUMINUM, 1000 },
			{ Resources::GOLD, 3000 },
			{ Resources::URANIUM, 3000 },
			{ Resources::TITANIUM, 8000 },
	}
	));
	this->planets.insert(createPlanet(glm::vec3(1000.0f * 5.0f, 0.0f, 0.0f), "Mars", 0.1f / 10.0f, 27.0f * 2.5f, MARS, slots,
		std::unordered_map<Resources::Type, int> {
			{ Resources::ALUMINUM, 1000 },
			{ Resources::URANIUM, 5000 },
			{ Resources::TITANIUM, 5000 },
			{ Resources::NANOMATERIAL, 5000 }
	}
	));
	this->planets.insert(createPlanet(glm::vec3(1800.0f * 5.0f, 0.0f, 0.0f), "Jupiter", 0.08f / 10.0f, 40.0f * 2.5f, JUPITER, slots,
		std::unordered_map<Resources::Type, int> {
			{ Resources::ALUMINUM, 1000 },
			{ Resources::GOLD, 800 },
			{ Resources::URANIUM, 5000 },
			{ Resources::TITANIUM, 5000 },
			{ Resources::NANOMATERIAL, 5000 }
	}
	));
}

void Universe::doLogic() {
	this->planet_updates.clear();
	for (auto& planet : this->planets) {
		planet->doLogic();
		this->planet_updates.push_back(planet->makeUpdate());
	}
}

std::set<std::unique_ptr<Planet>>& Universe::get_planets() {
	return this->planets;
}

std::vector<std::shared_ptr<PlanetUpdate>>& Universe::get_updates() {
	return this->planet_updates;
}
