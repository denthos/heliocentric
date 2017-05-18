#include "gui.h"

GUI::GUI(GLFWwindow * window) : Screen() {
	this->initialize(window, false);

	int ivar = 0;

	FormHelper * gui = new FormHelper(this);
	ref<Window> selectionWindow = gui->addWindow(Eigen::Vector2i(10, 10), "Selected Object");
	uidDisplay = gui->addVariable("UID:", ivar);
	uidDisplay->setTooltip("UID of the selected object.");
	uidDisplay->setEditable(false);

	this->setVisible(true);
	this->performLayout();
}

void GUI::updateSelection(GameObject * selected) {
	uidDisplay->setValue(selected->getID());
}