#pragma once
#include <nanogui\nanogui.h>
#include <vector>

#include "player.h"

using namespace nanogui;

class LeaderboardWidget : public Widget {
public:
	LeaderboardWidget(Widget* parent, std::string font, int font_size);
	~LeaderboardWidget();
	
	bool updateScoreEntry(const Player* player);

private:
	std::string font;
	int font_size;

	std::string formatScoreEntry(const Player* player);
	Label* createNewLabel(const Player* player);

	std::map<UID, Label*> playerLabels;
};