#include "player_color.h"

#include <unordered_map>

std::unordered_map<PlayerColor::Color, glm::vec3> PlayerColor::colorMap{
	{PlayerColor::Color::WHITE, glm::vec3(1.0f, 1.0f, 1.0f) },
	{PlayerColor::Color::RED, glm::vec3(1.0f, 0.0f, 0.0f)},
	{PlayerColor::Color::GREEN, glm::vec3(0.0f, 1.0f, 0.0f)},
	{PlayerColor::Color::BLUE, glm::vec3(0.0f, 0.0f, 1.0f)},
	{PlayerColor::Color::YELLOW, glm::vec3(1.0f, 1.0f, 0.0f)},
	{PlayerColor::Color::ORANGE, glm::vec3(1.0f, 0.3f, 0.0f)}
};

glm::vec3 PlayerColor::colorToRGBVec(PlayerColor::Color color) {
	auto& color_pair = colorMap.find(color);
	if (color_pair != colorMap.end()) {
		return color_pair->second;
	}
	else {
		return colorMap.begin()->second;
	}
}
