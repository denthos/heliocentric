#pragma once

#include "gui_utilities.h"
#include "unit_window.h"


#include "game_object.h"
#include "resources.h"
#include "player.h"

#include <nanogui\nanogui.h>
#include <unordered_map>

#if defined(NANOGUI_GLAD)
	#if defined(NANOGUI_SHARED) && !defined(GLAD_GLAPI_EXPORT)
		#define GLAD_GLAPI_EXPORT
	#endif

	#include <glad/glad.h>
#else
	#if defined(__APPLE__)
		#define GLFW_INCLUDE_GLCOREARB
	#else
		#define GL_GLEXT_PROTOTYPES
	#endif
#endif


using namespace nanogui;

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


	void displayCityUI(City* city, std::function<void()> createUnitCallback);
	void hideCityUI();

	void createTradeDisplay();
	void customizeTrade(Player* my_player, Player* trade_partner);
	void hideTradeUI();
	void showUnitUI(AttackableGameObject* unit);
	void hideUnitUI();

private:
	int screenWidth, screenHeight;
	UnitWindow* unit_window;
	FormHelper* formHelper;

	void createUidDisplay();
	void createSlotDisplay();
	void createCityDisplay();
	void createTradeDisplay(Player* my_player, Player* trade_partner);
	void createPlayerOverlay();

	std::pair<int, std::string> placeholderImage;

	ref<Window> uidWindow;
	detail::FormWidget<int> * uidDisplay;

	ref<Window> slotWindow;
	Button* slotButton;

	std::string cityName = "Default CityName";
	detail::FormWidget<std::string>* cityNameDisplay;

	Widget* slotResourcesWidget;
	std::unordered_map<Resources::Type, detail::FormWidget<int>*> resourceDisplay;

	ref<Window> cityWindow;
	Button* createUnitButton;

	Window * playerOverlay;
	std::shared_ptr<Player> player;
	std::pair<Resources::Type, Label *> resourceLabels[Resources::NUM_RESOURCES];
	Label * fpsSpacer;
	Label * fpsDisplay;

	ref<Window> tradeWindow;
	Button* createTradeButton;
};
