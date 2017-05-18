#include "gui.h"

GUI::GUI(GLFWwindow * window) : Screen() {
	this->initialize(window, false);
	unit_gui = new UnitGUI((Screen*) this, "Unit Stats");

	this->performLayout();
}

void GUI::updateSelection(GameObject * selected) {
	//update unit ui display
	AttackableGameObject* unit = dynamic_cast<AttackableGameObject*>(selected);
	if (unit) {
		unit_gui->updateSelection(selected);
		unit_gui->show();
	}
}


