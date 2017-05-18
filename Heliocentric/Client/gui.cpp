#include "gui.h"

GUI::GUI(GLFWwindow * window) : Screen() {
	this->initialize(window, false);
	gui = new FormHelper(this);

	/*
	int ivar = 0;

	

	
	ref<Window> selectionWindow = gui->addWindow(Eigen::Vector2i(10, 10), "Selected Object");
	uidDisplay = gui->addVariable("UID:", ivar);
	
	*/
}

void GUI::updateSelection(GameObject * selected) {
	//uidDisplay->setValue(selected->getID());
}

void GUI::show()
{
	userDisplay->setVisible(true);
	hidden = false;
}

void GUI::hide()
{
	userDisplay->setVisible(false);
	hidden = true;
}

bool GUI::isHidden()
{
	return hidden;
}
