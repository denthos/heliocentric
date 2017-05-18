#pragma once
#include "gui_utilities.h"
#include "unit_gui.h"
using namespace nanogui;
class GUI : public Screen {
public:
	GUI(GLFWwindow *); 
	~GUI();
	void updateSelection(GameObject * selected);

private:
	UnitGUI* unit_gui;

};