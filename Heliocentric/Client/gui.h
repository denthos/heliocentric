#pragma once

#include "gui_utilities.h"
#include "attackable_gameobject_widget.h"

#include "game_object.h"
#include "resources.h"
#include "player.h"
#include "unit_type.h"
#include "unit_spawn_widget.h"
#include "leaderboard_widget.h"
#include "slot_info_panel.h"
#include "unit_type.h"
#include "unit_create_button.h"
#include "tech_preview_widget.h"
#include "tech_tree_widget.h"

#include <nanogui\nanogui.h>
#include <unordered_map>

class Slot;
class Client;
class City;

class GUI : public Screen {
public:

	
	GUI(GLFWwindow *, std::function<void(std::shared_ptr<TradeData>)> tradeButtonCallback, std::function<void(UID, bool)> tradeHandlerCallback, 
			std::function<void(const Technology*)> techResearchCallback, int screenWidth, int screenHeight);
	~GUI();

	void update();

	void setScreenSize(int width, int height);

	void setPlayer(std::shared_ptr<Player>);
	void setFPS(double);

	void unselectSelection(Client*, std::vector<GameObject*>& old_selection);
	void selectSelection(Client*, std::vector<GameObject*>& old_selection);

	void displaySlotUI(Slot* slot, std::function<void(std::string)> createCityCallback);
	void hideSlotUI();


	void displayCityUI(City* city, std::function<void(UnitType*)> createUnitCallback);
	void hideCityUI();

	void createTradeDisplay();
	void createCustomTradeUI();
	void updateCustomTradeUI();
	void showCustomTradeUI();
	void hideCustomTradeUI();
	void showUnitUI(AttackableGameObject* unit);
	void hideUnitUI();
	void createTradeHandlerUI();
	void showTradeHandlerUI(std::shared_ptr<Player> sender, std::shared_ptr<TradeData> data);
	void hideTradeHandlerUI();
	void updateTradeHandlerUI(std::shared_ptr<Player> sender, std::shared_ptr<TradeData> data);

	void showGameOverWindow(bool victorious);
	void hideGameOverWindow();
	void updatePlayerLeaderboardValue(const Player* player);
	void addPlayer(std::shared_ptr<Player> new_player);

	void showChooseTechWindow();

private:
	int screenWidth, screenHeight;
	void updatePlayerOverlay();

	AttackableGameObject* selectedUnit = NULL;
	void updateUnitWindow();

	FormHelper* formHelper;
	std::vector<std::shared_ptr<Player>> players;

	std::function<void(std::shared_ptr<TradeData>)> tradeCallback;
	std::function<void(UID, bool)> tradeHandlerCallback;
	Client* client;

	void createUidDisplay();
	void createSlotDisplay();
	void createCityDisplay();
	void createPlayerOverlay();
	void createGameOverWindow();
	void createLeaderboardWindow();
	void createUnitDisplay();
	void createTechTreePreviewWindow();
	void createTechTreeWindow(std::function<void(const Technology*)> techResearchCallback);

	std::pair<int, std::string> placeholderImage;

	Window* unit_window;
	AttackableGameObjectWidget* unit_info_widget;

	ref<Window> uidWindow;
	detail::FormWidget<int> * uidDisplay;

	ref<Window> slotWindow;
	Button* slotButton;

	std::string cityName = "Default CityName";
	detail::FormWidget<std::string>* cityNameDisplay;

	SlotInfoPanel* slotInfoPanel;

	City* selectedCity = NULL;
	ref<Window> cityWindow;
	AttackableGameObjectWidget* cityInfoWidget;
	UnitSpawnWidget* unitSpawnWidget;
	SlotInfoPanel* citySlotInfoPanel;
	void updateCityWindow();
	void updateSlotWindow();

	ref<Window> gameOverWindow;
	Label* gameOverLabel;

	Window * playerOverlay;
	std::shared_ptr<Player> player;
	std::shared_ptr<Player> trade_partner;
	std::pair<Resources::Type, Label *> resourceLabels[Resources::NUM_RESOURCES];
	Label * fpsSpacer;
	Label * fpsDisplay;

	Window* leaderboardWindow;
	LeaderboardWidget* leaderboardWidget;

	Window* techPreviewWindow;
	TechPreviewWidget* techPreviewWidget;
	void updateTechTreePreviewWindow();

	Window* techTreeWindow;
	TechTreeWidget* techTreeWidget;
	void updateTechTreeWindow();

	// Everything Trade
	ref<Window> tradeWindow;
	ref<Window> customTradeWindow;
	ref<Window> tradeHandlerWindow;
	std::shared_ptr<TradeData> currentTradeData;
	Button* createTradeButton;
	Button* sendTradeButton;
	Button* closeTradeButton;
	float offerBaseVal = 0;
	Label* tradeHandlerLabel;

	// custom trade
	Widget* selectTradePartnerPanel;
	ComboBox* selectPartnerBox;
	Widget* tradePanel;
	IntBox<int>* offerAmount;
	ComboBox* offerResourceType;
	IntBox<int>* askForAmount;
	ComboBox* askForResourceType;
};
