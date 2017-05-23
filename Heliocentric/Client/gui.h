#pragma once
#include "gui_utilities.h"
#include "unit_window.h"
using namespace nanogui;

class Slot;
class Client;

class GUI : public Screen {
public:
	GUI(GLFWwindow *);
	~GUI();

	void unselectSelection(Client*, std::vector<GameObject*>& old_selection);
	void selectSelection(Client*, std::vector<GameObject*>& old_selection);

	void displaySlotUI(Slot* slot, std::function<void()> createCityCallback);
	void hideSlotUI();

	void showUnitUI(AttackableGameObject* unit);
	void hideUnitUI();

private:
	UnitWindow* unit_gui;
	FormHelper* formHelper;

	void createUidDisplay();
	void createSlotDisplay();

	ref<Window> uidWindow;
	detail::FormWidget<int> * uidDisplay;

	ref<Window> slotWindow;
	Button* slotButton;
};
