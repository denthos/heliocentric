#pragma once

#include "game_object.h"

#include <nanogui\nanogui.h>

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
	GUI(GLFWwindow *);

	void unselectSelection(Client*, std::vector<GameObject*>& old_selection);
	void selectSelection(Client*, std::vector<GameObject*>& old_selection);

	void displaySlotUI(Slot* slot, std::function<void()> createCityCallback);
	void hideSlotUI();

	void displayCityUI(City* city, std::function<void()> createUnitCallback);
	void hideCityUI();

	void displayTradeUI();
	void hideTradeUI();

private:

	FormHelper* formHelper;

	void createUidDisplay();
	void createSlotDisplay();
	void createCityDisplay();
	void createTradeDisplay();


	ref<Window> uidWindow;
	detail::FormWidget<int> * uidDisplay;

	ref<Window> slotWindow;
	Button* slotButton;

	ref<Window> cityWindow;
	Button* createUnitButton;

	ref<Window> tradeWindow;
	Button* createTradeButton;
};