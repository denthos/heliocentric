#pragma once
#include <glm/vec3.hpp>
#include <unordered_map>

class PlayerColor {
public:
	enum Color {
		WHITE,
		RED,
		BLUE,
		GREEN,
		YELLOW,
		ORANGE
	};

	const static Color FIRST = WHITE;
	const static int NUM_COLORS = 6;

	static glm::vec3 colorToRGBVec(PlayerColor::Color color);

private:
	static std::unordered_map<PlayerColor::Color, glm::vec3> colorMap;
};
