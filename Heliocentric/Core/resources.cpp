#include "resources.h"

// TODO: Move these data->string mappings to the client config file
std::unordered_map < Resources::Type, std::string> Resources::nameMap  = {
		{ALUMINUM, "Aluminum"},
		{GOLD, "Gold"},
		{NANOMATERIAL, "Nanomaterial"},
		{TITANIUM, "Titanium"},
		{URANIUM, "Uranium"}
	};

std::string Resources::toString(Resources::Type type) {
	return Resources::nameMap[type];
}
