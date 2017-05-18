#pragma once
#include "gui_utilities.h"
#include "unit_gui.h"
using namespace nanogui;
class GUI : public Screen {
public:
	GUI(GLFWwindow *);
	void updateSelection(GameObject * selected);
	UnitGUI* unit_gui;

protected:
	
private:
	
	

};