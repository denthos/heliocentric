#include "leaderboard_widget.h"
#include "player_color.h"

LeaderboardWidget::LeaderboardWidget(Widget* parent, std::string font, int font_size) :
	Widget(parent), font(font), font_size(font_size) {
	this->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill));
	this->setWidth(200);
}

LeaderboardWidget::~LeaderboardWidget() {
	for (auto& entry : playerLabels) {
		delete entry.second;
	}
}

std::string LeaderboardWidget::formatScoreEntry(const Player* player) {
	std::ostringstream sstream;
	sstream << std::left << std::setw(PLAYER_NAME_MAX_SIZE) << player->get_name() << ": ";
	sstream << std::left << std::setw(5) << player->get_player_score();

	return sstream.str();
}

Label* LeaderboardWidget::createNewLabel(const Player* player) {
	this->setHeight(this->height() + 30);
	std::string defaultLabel = "________________: _____";

	Label* playerLabel = new Label(this, defaultLabel, font, font_size);

	glm::vec3 rgbVec = PlayerColor::colorToRGBVec(player->getColor());
	playerLabel->setColor(Color(Eigen::Vector3f(rgbVec.x, rgbVec.y, rgbVec.z)));

	this->playerLabels.insert(std::make_pair(player->getID(), playerLabel));

	return playerLabel;
}

bool LeaderboardWidget::updateScoreEntry(const Player* player) {
	std::string newEntry = formatScoreEntry(player);
	bool madeNewLabel = false;

	Label* playerLabel;
	auto entry_it = playerLabels.find(player->getID());
	if (entry_it == playerLabels.end()) {
		madeNewLabel = true;
		playerLabel = createNewLabel(player);
	}
	else {
		playerLabel = entry_it->second;
	}

	playerLabel->setCaption(newEntry);
	return madeNewLabel;
}