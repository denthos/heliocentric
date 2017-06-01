#include "gui.h"
#include "slot.h"
#include "planet.h"
#include "selectable.h"
#include "city.h"
#include "resources.h"
#include "unit_spawner.h"

#define RESOURCE_IMAGE_DIRECTORY "Images/Resources"
#define PIXELS_PER_CHARACTER 14
#define FONT "courier"
#define FONT_SIZE 26
#define FONT_FILE "Fonts/Courier.ttf"
#define MAX_RESOURCE_CHARACTERS 9

GUI::GUI(GLFWwindow * window, int screenWidth, int screenHeight) : Screen(), screenWidth(screenWidth), screenHeight(screenHeight) {
	this->initialize(window, false);
	unit_window = new UnitWindow((Screen*) this, "Unit Stats");

	// load placeholder image
	std::vector<std::pair<int, std::string>> placeholderL = loadImageDirectory(nvgContext(), "Images/Placeholder");
	if (placeholderL.size() == 0) {
		LOG_WARN("Could not load placeholder image");
	}
	else {
		placeholderImage = placeholderL[0];
	}
	formHelper = new FormHelper(this);

	// load font
	nvgCreateFont(this->mNVGContext, FONT, FONT_FILE);

	this->createUidDisplay();
	this->createSlotDisplay();
	this->createCityDisplay();
	this->createPlayerOverlay();
	this->createGameOverWindow();
	this->createLeaderboardWindow();

	this->setVisible(true);
	this->performLayout();

	setScreenSize(screenWidth, screenHeight);
}

void GUI::update() {
	if (player) {
		for (std::pair<Resources::Type, Label *> resourceLabel : resourceLabels) {
			resourceLabel.second->setCaption(std::to_string((int)player->get_resource_amount(resourceLabel.first)));
		}
	}

	updateCityWindow();
	updateUnitWindow();
}



GUI::~GUI()
{
	delete unit_window;
}

void GUI::updateUnitWindow() {
	if (selectedUnit) {
		unit_window->updateSelection(selectedUnit);
	}
}


void GUI::showUnitUI(AttackableGameObject* unit) {
	selectedUnit = unit;
	this->unit_window->updateSelection(unit);
	this->unit_window->setVisible(true);
}

void GUI::hideUnitUI() {
	selectedUnit = NULL;
	this->unit_window->setVisible(false);
}

void GUI::setScreenSize(int width, int height) {
	this->screenWidth = width;
	this->screenHeight = height;
	if (playerOverlay) {
		playerOverlay->setSize(Eigen::Vector2i(screenWidth, 35));

		std::ostringstream spacerCaption;
		// spaceToFill = max width - resource images/labels - fps label - margins
		int spaceToFill = screenWidth - (Resources::NUM_RESOURCES * (6 + 25 + (MAX_RESOURCE_CHARACTERS * PIXELS_PER_CHARACTER))) - (3 + (9 * PIXELS_PER_CHARACTER)) - 20;
		for (int i = 0; i < spaceToFill / PIXELS_PER_CHARACTER; ++i) {
			spacerCaption << "a";
		}
		fpsSpacer->setCaption(spacerCaption.str());

		playerOverlay->performLayout(nvgContext());
	}
}

void GUI::setPlayer(std::shared_ptr<Player> player) {
	this->player = player;
	this->updatePlayerLeaderboardValue(player.get());
}

void GUI::setFPS(double fps) {
	std::ostringstream oss;
	oss << "FPS: " << (int)fps;
	this->fpsDisplay->setCaption(oss.str());

}

void GUI::createUidDisplay() {
	int ivar = 0;
	uidWindow = formHelper->addWindow(Eigen::Vector2i(10, 40), "Selected Object");

	uidDisplay = formHelper->addVariable("UID:", ivar);
	uidDisplay->setTooltip("UID of the selected object.");
	uidDisplay->setEditable(false);
}

void GUI::createSlotDisplay() {
	slotWindow = formHelper->addWindow(Eigen::Vector2i(10, 120), "Selected Planet");
	slotWindow->setWidth(200);



	formHelper->addGroup("Slot Info");
	this->slotInfoPanel = new SlotInfoPanel(slotWindow);
	formHelper->addWidget("", this->slotInfoPanel);

	formHelper->addGroup("Found a City");
	// TODO: Make this a max of 16 characters
	cityNameDisplay = formHelper->addVariable("Name", cityName);

	slotButton = formHelper->addButton("Establish City", []() {});
	slotWindow->setVisible(false);
}

void GUI::createPlayerOverlay() {
	playerOverlay = new Window(this, "Player Overlay");
	Color fontColor = playerOverlay->theme()->mTextColor;
	Theme* playerOverlayTheme = new Theme(*playerOverlay->theme());
	playerOverlayTheme->mWindowFillFocused = Color(0.1f, 0.1f, 0.1f, 1.0f);
	playerOverlayTheme->mWindowFillUnfocused = playerOverlayTheme->mWindowFillFocused;
	playerOverlayTheme->mBorderDark = Color(0.1f, 0.1f, 0.1f, 1.0f);
	playerOverlayTheme->mBorderLight = playerOverlayTheme->mBorderDark;
	playerOverlayTheme->mWindowHeaderHeight = 0;
	playerOverlay->setTheme(playerOverlayTheme);
	playerOverlay->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Minimum, 10, 3));
	playerOverlay->setPosition(Eigen::Vector2i(0, -8));
	playerOverlay->setVisible(true);
	std::string resourceImageDirectory = std::string(RESOURCE_IMAGE_DIRECTORY);
	std::vector<std::pair<int, std::string>> icons = loadImageDirectory(nvgContext(), resourceImageDirectory);
	if (icons.size() < Resources::NUM_RESOURCES) {
		LOG_WARN("Could not find all of the resource icons");
		while (icons.size() < Resources::NUM_RESOURCES) {
			icons.push_back(placeholderImage);
		}
	}
	for (int i = Resources::FIRST; i < Resources::NUM_RESOURCES; ++i) {
		std::string resourceName = Resources::toString((Resources::Type)i);
		int image = placeholderImage.first;
		for (std::pair<int, std::string> icon : icons) {
			if (icon.second.compare(resourceImageDirectory + "/" + resourceName) == 0) {
				image = icon.first;
				break;
			}
		}
		ImageView * resourceImage = new ImageView(playerOverlay, image);
		resourceImage->setTooltip(resourceName);
		resourceImage->setFixedSize(Eigen::Vector2i(25, 25));
		resourceImage->setFixedOffset(true); //DONT DRAG THE ICONS
		Label * resourceLabel = new Label(playerOverlay, "0", FONT, FONT_SIZE);
		resourceLabel->setTooltip(resourceName);
		resourceLabel->setFixedWidth(MAX_RESOURCE_CHARACTERS * PIXELS_PER_CHARACTER);
		resourceLabels[i] = std::make_pair((Resources::Type)i, resourceLabel);
	}
	playerOverlay->theme()->mTextColor = playerOverlay->theme()->mWindowFillFocused;
	fpsSpacer = new Label(playerOverlay, "", FONT, FONT_SIZE);
	playerOverlay->theme()->mTextColor = fontColor;
	fpsDisplay = new Label(playerOverlay, "FPS: ", FONT, FONT_SIZE);
	fpsDisplay->setTooltip("Frames per second");
	fpsDisplay->setFixedWidth(9 * PIXELS_PER_CHARACTER);
}

void GUI::createGameOverWindow() {
	gameOverWindow = formHelper->addWindow(Eigen::Vector2i(0, 0), "GAME OVER");
	gameOverLabel = new Label(gameOverWindow, "YOU ARE VICTORIOUS!", FONT, FONT_SIZE);

	formHelper->addWidget("", gameOverLabel);
	gameOverWindow->center();
	gameOverWindow->setVisible(false);
}

void GUI::showGameOverWindow(bool victorious) {
	std::string message = victorious ? "VICTORIOUS" : "A LOSER";
	gameOverLabel->setCaption("YOU ARE " + message);
	gameOverWindow->setVisible(true);
}

void GUI::hideGameOverWindow() {
	gameOverWindow->setVisible(false);
}


void GUI::createLeaderboardWindow() {
	this->leaderboardWindow = formHelper->addWindow(Eigen::Vector2i(screenWidth - 200, 140), "Leaderboard");
	this->leaderboardWidget = new LeaderboardWidget(leaderboardWindow);
	this->leaderboardWindow->setWidth(this->leaderboardWidget->width());
	formHelper->addWidget("", leaderboardWidget);
	this->leaderboardWindow->performLayout(nvgContext());
}

void GUI::updatePlayerLeaderboardValue(const Player* player) {
	if (this->leaderboardWidget->updateScoreEntry(player)) {
		this->leaderboardWindow->setHeight(this->leaderboardWidget->height() + 40);
		this->leaderboardWindow->performLayout(nvgContext());
	}
}

void GUI::unselectSelection(Client* client, std::vector<GameObject*>& old_selection) {
	/* For now, let's just unselect() all selectables */
	for (GameObject* single_object : old_selection) {
		Selectable* selectable_object = dynamic_cast<Selectable*>(single_object);
		if (selectable_object) {
			selectable_object->unselect(this, client);
		}
	}
}

void GUI::selectSelection(Client* client, std::vector<GameObject*>& new_selection) {
	/* For now, let's just select() all selectables */
	for (GameObject* single_object : new_selection) {
		Selectable* selectable_object = dynamic_cast<Selectable*>(single_object);
		if (selectable_object) {
			selectable_object->select(this, client);
		}

		uidDisplay->setValue(single_object->getID());
	}

}

void GUI::createCityDisplay() {
	cityWindow = formHelper->addWindow(Eigen::Vector2i(10, 120), "City Name");

	formHelper->addGroup("Slot Info");
	citySlotInfoPanel = new SlotInfoPanel(cityWindow);
	formHelper->addWidget("", citySlotInfoPanel);

	formHelper->addGroup("Unit Management");
	unitSpawnWidget = new UnitSpawnWidget(cityWindow);
	formHelper->addWidget("", unitSpawnWidget);

	cityWindow->setVisible(false);
}


void GUI::updateCityWindow() {
	if (cityWindow->visible()) {
		unitSpawnWidget->updateSelection(selectedCity);
	}
}

void GUI::displaySlotUI(Slot* slot, std::function<void(std::string)> createCityCallback) {
	slotWindow->setTitle(slot->getPlanet()->getName());
	slotButton->setCallback([createCityCallback, this]() {
		createCityCallback(cityName);
		slotWindow->setVisible(false);
	});

	slotInfoPanel->updateDisplay(slot);

	slotWindow->setVisible(true);
}

void GUI::hideSlotUI() {
	slotWindow->setVisible(false);
}

void GUI::displayCityUI(City* city, std::function<void(UnitType*)> unitCreateCallback) {
	selectedCity = city;
	cityWindow->setTitle(city->getName());
	citySlotInfoPanel->updateDisplay(city->get_slot());
	cityWindow->setVisible(true);

	/* Only show the spawn ui if the owner clicked the city */
	if (this->player->getID() == city->get_player()->getID()) {
		unitSpawnWidget->setVisible(true);
		unitSpawnWidget->setCreateButtonCallback(unitCreateCallback);
	}
	else {
		unitSpawnWidget->setVisible(false);
	}
}

void GUI::hideCityUI() {
	cityWindow->setVisible(false);
}

