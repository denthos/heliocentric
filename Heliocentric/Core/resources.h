#pragma once

#include <unordered_map>

class Resources {
public:
	enum Type {ALUMINUM, GOLD, NANOMATERIAL, TITANIUM, URANIUM};

	static const Type FIRST = ALUMINUM;
	static const unsigned int NUM_RESOURCES = 5;

	static std::string toString(Resources::Type);
private:
	static std::unordered_map < Resources::Type, std::string> nameMap;
};

typedef std::unordered_map<Resources::Type, int> ResourceCollection;
