#include "gui.h"
#include "slot.h"
#include "planet.h"
#include "selectable.h"
#include "city.h"
#include "resources.h"
#include "builder.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

#define RESOURCE_IMAGE_DIRECTORY "Images/Resources"
#define ICON_IMAGE_DIRECTORY "Images/Icons"
#define PIXELS_PER_CHARACTER 14
#define LARGE_FONT "courier"
#define LARGE_FONT_SIZE 26
#define STANDARD_FONT "sans"
#define STANDARD_FONT_SIZE 20
#define FONT_FILE "Fonts/Courier.ttf"
#define MAX_RESOURCE_CHARACTERS 9

GUI::GUI(GLFWwindow * window, std::function<void(std::shared_ptr<TradeData>)> tradeCallback, std::function<void(UID, bool)> tradeHandlerCallback, std::function<void(const Technology*)> techResearchCallback, int screenWidth, int screenHeight) : 
	Screen(), tradeCallback(tradeCallback), tradeHandlerCallback(tradeHandlerCallback), screenWidth(screenWidth), screenHeight(screenHeight) {

	this->initialize(window, false);

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
	nvgCreateFont(this->mNVGContext, LARGE_FONT, FONT_FILE);

	this->createUidDisplay();
	this->createSlotDisplay();
	this->createCityDisplay();
	this->createPlayerOverlay();
	this->createTradeDisplay();
	this->createCustomTradeUI();
	this->createTradeHandlerUI();
	this->createHelpWindow();
	
	this->createGameOverWindow();
	this->createLeaderboardWindow();
	this->createUnitDisplay();
	this->createTechTreePreviewWindow();
	this->createTechTreeWindow(techResearchCallback);

	this->setVisible(true);
	this->performLayout();

	setScreenSize(screenWidth, screenHeight);
}


void GUI::update() {
	updatePlayerOverlay();
	updateCityWindow();
	updateSlotWindow();
	updateUnitWindow();
	updateTechTreePreviewWindow();
	updateTechTreeWindow();
}


GUI::~GUI()
{
	delete unit_window;
}

void GUI::updatePlayerOverlay() {
	if (player) {
		for (std::pair<Resources::Type, Label *> resourceLabel : resourceLabels) {
			resourceLabel.second->setCaption(std::to_string((int)player->get_resource_amount(resourceLabel.first)));
		}
	}
}

void GUI::createTechTreePreviewWindow() {
	this->techPreviewWindow = formHelper->addWindow(Eigen::Vector2i(screenWidth - 100, screenHeight - 200), "Technology");

	std::function<void()> previewCallback = std::bind(&GUI::showChooseTechWindow, this);
	this->techPreviewWidget = new TechPreviewWidget(techPreviewWindow, STANDARD_FONT, STANDARD_FONT_SIZE, previewCallback);
	formHelper->addWidget("", techPreviewWidget);
}

void GUI::createTechTreeWindow(std::function<void(const Technology*)> techResearchCallback) {
	this->techTreeWindow = formHelper->addWindow(Eigen::Vector2i(0, 0), "Research");
	this->techTreeWidget = new TechTreeWidget(techTreeWindow, [this, techResearchCallback](const Technology* tech) {
		this->techTreeWindow->performLayout(nvgContext());
		techResearchCallback(tech);
	});
	formHelper->addWidget("", techTreeWidget);

	Button* closeButton = new Button(techTreeWindow->buttonPanel(), "X");
	closeButton->setCallback([this]() {
		techTreeWindow->setVisible(false);
	});

	techTreeWindow->setVisible(false);
}


void GUI::updateTechTreePreviewWindow() {
	if (this->techPreviewWindow->visible() && player) {
		this->techPreviewWidget->updatePreview(&player->getTechTree());
		techPreviewWindow->performLayout(nvgContext());
	}
}

void GUI::showChooseTechWindow() {
	techTreeWidget->updateTechTreeWidget(&player->getTechTree());
	techTreeWindow->setSize(Eigen::Vector2i(techTreeWidget->width(), techTreeWidget->height() + 20));
	techTreeWindow->performLayout(nvgContext());
	techTreeWindow->setVisible(true);
	techTreeWindow->center();
}

void GUI::updateTechTreeWindow() {
	// Only do things if we are looking at the tech tree window
	if (techTreeWindow->visible()) {
		techTreeWidget->updateTechTreeWidget(&player->getTechTree());
		techTreeWindow->performLayout(nvgContext());
	}
}

void GUI::updateUnitWindow() {
	if (selectedUnit) {
		unit_info_widget->updateSelection(selectedUnit);
		unit_window->performLayout(nvgContext());
	}
}


void GUI::showUnitUI(AttackableGameObject* unit) {
	selectedUnit = unit;

	Unit* cast_unit = dynamic_cast<Unit*>(unit);
	if (cast_unit) {
		this->unit_window->setTitle(cast_unit->getType()->getTypeName());
	}

	updateUnitWindow();
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

		/*
		std::ostringstream spacerCaption;
		// spaceToFill = max width - resource images/labels - fps label - margins
		int spaceToFill = screenWidth - (Resources::NUM_RESOURCES * (6 + 25 + (MAX_RESOURCE_CHARACTERS * PIXELS_PER_CHARACTER))) - (3 + (9 * PIXELS_PER_CHARACTER)) - 20;
		for (int i = 0; i < spaceToFill / PIXELS_PER_CHARACTER; ++i) {
			spacerCaption << "a";
		}
		fpsSpacer->setCaption(spacerCaption.str());
		*/

		playerOverlay->performLayout(nvgContext());
	}
}

void GUI::setPlayer(std::shared_ptr<Player> player) {
	this->player = player;
	this->updatePlayerLeaderboardValue(player.get());
}

void GUI::setFPS(double fps) {
#ifdef _DEBUG
	std::ostringstream oss;
	oss << "FPS: " << (int)fps;
	this->fpsDisplay->setCaption(oss.str());
#endif
}

void GUI::setResearchPoints(int researchPoints) {
	std::ostringstream oss;
	oss << researchPoints;
	this->researchPointsDisplay->setCaption(oss.str());
}

void GUI::setTimer(int timer) {
	if (timer == time || timer < 0)
		return;
	std::ostringstream oss;
	oss << "Timer: " << timer;
	this->timerDisplay->setCaption(oss.str());
	time = timer;
}

void GUI::addPlayer(std::shared_ptr<Player> new_player) {
	players.push_back(new_player);

	std::vector<std::string> player_names(selectPartnerBox->items());
	player_names.push_back(new_player->get_name());
	selectPartnerBox->setItems(player_names);

	this->performLayout();
}

void GUI::createUidDisplay() {
#ifdef _DEBUG
	int ivar = 0;
	uidWindow = formHelper->addWindow(Eigen::Vector2i(10, 40), "Selected Object");

	uidDisplay = formHelper->addVariable("UID:", ivar);
	uidDisplay->setTooltip("UID of the selected object.");
	uidDisplay->setEditable(false);
#endif
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
		Label * resourceLabel = new Label(playerOverlay, "0", LARGE_FONT, LARGE_FONT_SIZE);
		resourceLabel->setTooltip(resourceName);
		resourceLabel->setFixedWidth(MAX_RESOURCE_CHARACTERS * PIXELS_PER_CHARACTER);
		resourceLabels[i] = std::make_pair((Resources::Type)i, resourceLabel);
	}

	/*
	playerOverlay->theme()->mTextColor = playerOverlay->theme()->mWindowFillFocused;
	fpsSpacer = new Label(playerOverlay, "", LARGE_FONT, LARGE_FONT_SIZE);
	playerOverlay->theme()->mTextColor = fontColor;
	*/
	int research_img = placeholderImage.first;
	for (std::pair<int, std::string> icon : icons) {
			if (icon.second.compare(resourceImageDirectory + "/" + "Research") == 0) {
				research_img = icon.first;
				break;
			}
		}

	researchImage = new ImageView(playerOverlay, research_img);
	researchImage->setTooltip("Total amount of research points you have. The more you have the faster you unlock new techs.");
	researchImage->setFixedSize(Eigen::Vector2i(25, 25));
	researchImage->setFixedOffset(true);
	researchPointsDisplay = new Label(playerOverlay, "0", LARGE_FONT, LARGE_FONT_SIZE);
	researchPointsDisplay->setTooltip("Total amount of research points you have. The more you have the faster you unlock new techs.");
	researchPointsDisplay->setFixedWidth(12 * PIXELS_PER_CHARACTER);

	timerDisplay = new Label(playerOverlay, "Timer: ", LARGE_FONT, LARGE_FONT_SIZE);
	timerDisplay->setTooltip("Time Remaining... Hurry!");
	timerDisplay->setFixedWidth(12 * PIXELS_PER_CHARACTER);

#ifdef _DEBUG
	fpsDisplay = new Label(playerOverlay, "FPS: ", LARGE_FONT, LARGE_FONT_SIZE);
	fpsDisplay->setTooltip("Frames per second");
	fpsDisplay->setFixedWidth(9 * PIXELS_PER_CHARACTER);
#endif
}

void GUI::createTradeDisplay() {
	LOG_DEBUG("Creating trade display");
	tradeWindow = formHelper->addWindow(Eigen::Vector2i(screenWidth - 200, 500), "Trade Deal");
	createTradeButton = formHelper->addButton("Establish Trade", []() {});

	createTradeButton->setCallback([this]() {
		showCustomTradeUI();
	});
}

void GUI::createCustomTradeUI() {
	customTradeWindow = formHelper->addWindow(Eigen::Vector2i(100, 100), "Customize Trade Window");
	closeTradeButton = new Button(customTradeWindow->buttonPanel(), "X");
	closeTradeButton->setCallback([this]() {
		this->hideCustomTradeUI();
	});
	customTradeWindow->setVisible(false);
	customTradeWindow->center();
	
	// set up player selection
	formHelper->addGroup("");
	selectTradePartnerPanel = new Widget(customTradeWindow);
	selectTradePartnerPanel->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 1, 2));
	selectPartnerBox = new ComboBox(selectTradePartnerPanel);
	selectPartnerBox->setItems({});
	formHelper->addWidget("Select Trade Partner: ", selectTradePartnerPanel);

	// set up trade panel for resource types and amounts
	tradePanel = new Widget(customTradeWindow);
	tradePanel->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 0, 2));

	Label* offerAmountLabel = new Label(tradePanel, " SELL ");
	offerAmount = new IntBox<int>(tradePanel);
	offerResourceType = new ComboBox(tradePanel);
	Label* buyAmountLabel = new Label(tradePanel, " BUY ");
	askForAmount = new IntBox<int>(tradePanel);
	askForResourceType = new ComboBox(tradePanel);

	selectPartnerBox->setCallback([this](int player_index) {
		this->trade_partner = this->players.at(player_index);
		LOG_DEBUG("Player index is " + std::to_string(player_index));
		std::vector<std::string> resource_list = {};
		for (auto itr : trade_partner->getResources()) {
			LOG_DEBUG("Resource type is " + Resources::toString(itr.first));
			resource_list.push_back(Resources::toString(itr.first));
		}
		askForAmount->setValue(0);
		askForResourceType->setItems(resource_list);
		this->performLayout(nvgContext());
	});

	offerAmount->setEditable(true);
	offerAmount->setCallback([this](int val) {
		if (val >= 0)
			offerAmount->setValue(val);
	});
	offerResourceType->setItems({});
	offerResourceType->setCallback([this](int val) {
		offerAmount->setMinMaxValues(0, this->player.get()->get_resource_amount(static_cast<Resources::Type>(val)));
		offerAmount->setValue(this->player.get()->get_resource_amount(static_cast<Resources::Type>(val)));
	});
	askForAmount->setEditable(true);
	askForAmount->setCallback([this](int val) {
		if (val >= 0)
			askForAmount->setValue(val);
	});
	askForResourceType->setItems({});
	askForResourceType->setCallback([this](int val) {
		askForAmount->setMinMaxValues(0, this->trade_partner->get_resource_amount(static_cast<Resources::Type>(val)));
		askForAmount->setValue(this->trade_partner->get_resource_amount(static_cast<Resources::Type>(val)));
	});
	formHelper->addWidget("Custom Trade Panel: ", tradePanel);

	sendTradeButton = formHelper->addButton("Send", []() {});

	sendTradeButton->setCallback([this]() {
		this->tradeCallback(std::make_shared<TradeData>(this->player->getID(), trade_partner->getID(), static_cast<Resources::Type>(offerResourceType->selectedIndex()),
			offerAmount->value(), static_cast<Resources::Type>(askForResourceType->selectedIndex()), askForAmount->value()));
		this->hideCustomTradeUI();
		this->performLayout();
	});

	this->performLayout();
}

void GUI::updateCustomTradeUI() {
	// set default trade partner to be first on player list
	if (this->players.size() > 0)
		this->trade_partner = this->players.at(0);
	else {
		LOG_ERR("Only one player is connected, trade function disabled.");
		return;
	}

	// update names
	std::vector<std::string> player_names = {};
	for (auto itr : players) {
		player_names.push_back(itr->get_name());
	}
	selectPartnerBox->setItems(player_names);

	std::vector<std::string> my_resource_list = {};
	std::vector<std::string> partner_resource_list = {};

	for (auto itr : this->player->getResources()) {
		my_resource_list.push_back(Resources::toString(itr.first));
	}

	for (auto itr : trade_partner->getResources()) {
		partner_resource_list.push_back(Resources::toString(itr.first));
	}

	offerResourceType->setItems(my_resource_list);
	askForResourceType->setItems(partner_resource_list);
	offerAmount->setMinMaxValues(0, this->player->get_resource_amount(static_cast<Resources::Type>(0)));
	askForAmount->setMinMaxValues(0, this->trade_partner->get_resource_amount(static_cast<Resources::Type>(0)));
	this->performLayout();
}

void GUI::showCustomTradeUI() {
	updateCustomTradeUI();
	tradeWindow->setVisible(false);
	customTradeWindow->setVisible(true);
}

void GUI::createTradeHandlerUI() {
	tradeHandlerWindow = formHelper->addWindow(Eigen::Vector2i(100, 100), "You have an offer!");
	tradeHandlerLabel = new Label(tradeHandlerWindow, "holder", LARGE_FONT, LARGE_FONT_SIZE);
	formHelper->addWidget("", tradeHandlerLabel);
	Button* acceptBtn = formHelper->addButton("Accept", []() {});
	Button* declineBtn = formHelper->addButton("Decline", []() {});
	acceptBtn->setCallback([this]() {
		LOG_DEBUG("Trade id is " + std::to_string(currentTradeData->trade_deal_id));
		tradeHandlerCallback(currentTradeData->trade_deal_id, true);
		hideTradeHandlerUI();
		LOG_DEBUG("I accepted trade deal.");
	});
	declineBtn->setCallback([this]() {
		LOG_DEBUG("Trade id is " + std::to_string(currentTradeData->trade_deal_id));
		tradeHandlerCallback(currentTradeData->trade_deal_id, false);
		hideTradeHandlerUI();
		LOG_DEBUG("I decline trade deal.");
	});
	tradeHandlerWindow->setVisible(false);
}

void GUI::showTradeHandlerUI(std::shared_ptr<Player> sender, std::shared_ptr<TradeData> data) {
	updateTradeHandlerUI(sender, data);
	tradeHandlerWindow->setVisible(true);
}

void GUI::hideTradeHandlerUI() {
	tradeHandlerWindow->setVisible(false);
}

void GUI::updateTradeHandlerUI(std::shared_ptr<Player> sender, std::shared_ptr<TradeData> data) {
	std::ostringstream oss;
	oss << sender->get_name() << " sends you " << std::to_string(data->sell_amount) << " " << Resources::toString(data->sell_type)
		<< " if you send them " << std::to_string(data->buy_amount) << " " << Resources::toString(data->buy_type);
	std::string output = oss.str();
	tradeHandlerLabel->setCaption(output);
	currentTradeData = data;
	LOG_DEBUG("Creating trade caption when window is available");

	// reorganize ui layout
	this->performLayout();
}


void GUI::hideCustomTradeUI() {
	customTradeWindow->setVisible(false);
	tradeWindow->setVisible(true);
}

void GUI::createGameOverWindow() {
	gameOverWindow = formHelper->addWindow(Eigen::Vector2i(0, 0), "GAME OVER");
	gameOverLabel = new Label(gameOverWindow, "YOU ARE VICTORIOUS!", LARGE_FONT, LARGE_FONT_SIZE);

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
	this->leaderboardWindow->setFontSize(STANDARD_FONT_SIZE);

	this->leaderboardWidget = new LeaderboardWidget(leaderboardWindow, STANDARD_FONT, STANDARD_FONT_SIZE);
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

#ifdef _DEBUG
		uidDisplay->setValue(single_object->getID());
#endif
	}

}

void GUI::createCityDisplay() {
	cityWindow = formHelper->addWindow(Eigen::Vector2i(10, 120), "City Name");

	formHelper->addGroup("Slot Info");
	citySlotInfoPanel = new SlotInfoPanel(cityWindow);
	formHelper->addWidget("", citySlotInfoPanel);

	formHelper->addGroup("City Stats");
	cityInfoWidget = new AttackableGameObjectWidget(cityWindow);
	formHelper->addWidget("", cityInfoWidget);

	formHelper->addGroup("Production");
	unitSpawnWidget = new UnitSpawnWidget(cityWindow);
	formHelper->addWidget("", unitSpawnWidget);

	cityWindow->setVisible(false);
}

void GUI::createUnitDisplay() {
	unit_window = formHelper->addWindow(Eigen::Vector2i(10, 120), "Unit Type Goes Here");

	formHelper->addGroup("Unit Information");
	unit_info_widget = new AttackableGameObjectWidget(unit_window);
	formHelper->addWidget("", unit_info_widget);

	unit_window->setVisible(false);
}


void GUI::updateCityWindow() {
	if (cityWindow->visible()) {
		unitSpawnWidget->updateSelection(selectedCity, this->player.get());
		cityInfoWidget->updateSelection(selectedCity);
		citySlotInfoPanel->updateDisplay(selectedCity->get_slot());
		cityWindow->performLayout(nvgContext());
	}
}


void GUI::updateSlotWindow() {
	if (slotWindow->visible()) {
		if (player->can_settle() != slotButton->enabled()) {
			if (player->can_settle()) {
				slotButton->setTooltip("");
			}
			else {
				slotButton->setTooltip("Reached settlement limit (" + std::to_string(player->get_settlement_limit()) + ")");
			}

		}
		slotButton->setEnabled(player->can_settle());
		slotWindow->performLayout(nvgContext());
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

void GUI::displayCityUI(City* city, std::function<void(Buildable*)> unitCreateCallback) {
	selectedCity = city;
	cityWindow->setTitle(city->getName());
	unitSpawnWidget->setCreateButtonCallback(unitCreateCallback);

	updateCityWindow();
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

void GUI::createHelpWindow() {
	helpWindow = formHelper->addWindow(Eigen::Vector2i(screenWidth - 200, 800), "Help");
	Button* helpButton = formHelper->addButton("Open", [this]() {showHelpDetailWindow(); });
	std::string icon_image_directory = std::string(ICON_IMAGE_DIRECTORY);
	std::vector<std::pair<int, std::string>> help_icons = loadImageDirectory(nvgContext(), icon_image_directory);
	helpButton->setIcon(0);
	helpDetailWindow = formHelper->addWindow(Eigen::Vector2i(500, 120), "Help Window");
	Button* closeHelpButton = new Button(helpDetailWindow->buttonPanel(), "X");
	closeHelpButton->setCallback([this]() {
		hideHelpDetailWindow();
	});
	//helpDetailWindow->setFixedWidth(200);
	Widget* gameGoalPanel = new Widget(helpDetailWindow);
	gameGoalPanel->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Middle, 10, 10));
	Label* explainGameGoal = new Label(gameGoalPanel, "Gain the most scores within the time restraint!", LARGE_FONT, STANDARD_FONT_SIZE);
	formHelper->addWidget("GOAL", gameGoalPanel);
	Widget* gameObjectivePanel = new Widget(helpDetailWindow);
	gameObjectivePanel->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Middle, 10, 10));
	Label* explainGameGoal1 = new Label(gameObjectivePanel, "1) Kill an enemy UNIT or CITY to gain points equivalent to their ATTACK.", LARGE_FONT, STANDARD_FONT_SIZE);
	Label* explainGameGoal2 = new Label(gameObjectivePanel, "2) Create city to excavate resources.", LARGE_FONT, STANDARD_FONT_SIZE);
	Label* explainGameGoal3 = new Label(gameObjectivePanel, "3) Perform RESEARCH to upgrade unit and cities.", LARGE_FONT, STANDARD_FONT_SIZE);
	Label* explainGameGoal4 = new Label(gameObjectivePanel, "4) TRADE with another player for unique RESOURCES.", LARGE_FONT, STANDARD_FONT_SIZE);
	formHelper->addWidget("OBJECTIVES", gameObjectivePanel);
	Widget* hotKeyPanel = new Widget(helpDetailWindow);
	hotKeyPanel->setLayout(new BoxLayout(Orientation::Horizontal, Alignment::Middle, 10, 10));
	TextBox* cameraKey = new TextBox(hotKeyPanel, "`");
	Label* cameraKeyLabel = new Label(hotKeyPanel, "Swap camera views", LARGE_FONT, STANDARD_FONT_SIZE);
	TextBox* escKey = new TextBox(hotKeyPanel, "Esc");
	Label* escKeyLabel = new Label(hotKeyPanel, "Exit game", LARGE_FONT, STANDARD_FONT_SIZE);
	TextBox* leftBracKey = new TextBox(hotKeyPanel, "[");
	Label* leftBracKeyLabel = new Label(hotKeyPanel, "Decrease Volume", LARGE_FONT, STANDARD_FONT_SIZE);
	TextBox* rightBracKey = new TextBox(hotKeyPanel, "]");
	Label* rightBracKeyLabel = new Label(hotKeyPanel, "Increase Volume", LARGE_FONT, STANDARD_FONT_SIZE);
	formHelper->addWidget("HOT KEYS", hotKeyPanel);
	Widget* techPanel = new Widget(helpDetailWindow);
	techPanel->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Minimum, 10, 10));
	Label* explainTech1 = new Label(techPanel, "1) STEEL PLATING.", LARGE_FONT, STANDARD_FONT_SIZE);
	Label* explainTech2 = new Label(techPanel, "2) Tech 2.", LARGE_FONT, STANDARD_FONT_SIZE);
	Label* explainTech3 = new Label(techPanel, "3) HEAVY UNIT. Unlocked by STEEL PLATING.", LARGE_FONT, STANDARD_FONT_SIZE);
	Label* explainTech4 = new Label(techPanel, "4) Tech 4. Unlocked by STEEL PLATING.", LARGE_FONT, STANDARD_FONT_SIZE);
	Label* explainTech5 = new Label(techPanel, "5) Exploration Program. Increase city settlement limit by 1. Unlocked by Tech 2.", LARGE_FONT, STANDARD_FONT_SIZE);
	Label* explainTech6 = new Label(techPanel, "6) SECRECT.", LARGE_FONT, LARGE_FONT_SIZE);
	formHelper->addWidget("RESEARCH TECHS", techPanel);
	this->performLayout();

	hideHelpDetailWindow();
}

void GUI::showHelpDetailWindow() {
	helpDetailWindow->setVisible(true);
	helpWindow->setVisible(false);
}
void GUI::hideHelpDetailWindow() {
	helpWindow->setVisible(true);
	helpDetailWindow->setVisible(false);
}

