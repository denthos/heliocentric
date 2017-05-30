#pragma once

#include "gui_utilities.h"
#include "unit_window.h"

#include "game_object.h"
#include "resources.h"
#include "player.h"
#include "slot_info_panel.h"
#include "unit_type.h"
#include "unit_create_button.h"

#include <nanogui\nanogui.h>
#include <unordered_map>

class Slot;
class Client;
class City;

class GUI : public Screen {
public:

	

	GUI(GLFWwindow *, int screenWidth, int screenHeight);
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

	void showUnitUI(AttackableGameObject* unit);
	void hideUnitUI();

private:
	int screenWidth, screenHeight;
	UnitWindow* unit_window;
	FormHelper* formHelper;

	void createUidDisplay();
	void createSlotDisplay();
	void createCityDisplay();
	void createPlayerOverlay();

	std::pair<int, std::string> placeholderImage;

	ref<Window> uidWindow;
	detail::FormWidget<int> * uidDisplay;

	ref<Window> slotWindow;
	Button* slotButton;

	std::string cityName = "Default CityName";
	detail::FormWidget<std::string>* cityNameDisplay;

	SlotInfoPanel* slotInfoPanel;

	City* selectedCity = NULL;
	ref<Window> cityWindow;
	SlotInfoPanel* citySlotInfoPanel;
	std::vector<UnitCreateButton*> createUnitButtons;
	ProgressBar* cityUnitCreateProgressBar;
	void updateCityWindow();

	Window * playerOverlay;
	std::shared_ptr<Player> player;
	std::pair<Resources::Type, Label *> resourceLabels[Resources::NUM_RESOURCES];
	Label * fpsSpacer;
	Label * fpsDisplay;

};
