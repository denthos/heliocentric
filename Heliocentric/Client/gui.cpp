#include "gui.h"
#include "slot.h"
#include "planet.h"
#include "selectable.h"
#include "city.h"

GUI::GUI(GLFWwindow * window) : Screen() {
	this->initialize(window, false);

	formHelper = new FormHelper(this);

	this->createUidDisplay();
	this->createSlotDisplay();
	this->createCityDisplay();
	this->createTradeDisplay();

	this->setVisible(true);
	this->performLayout();
}


void GUI::createUidDisplay() {
	int ivar = 0;
	uidWindow = formHelper->addWindow(Eigen::Vector2i(10, 10), "Selected Object");
	uidDisplay = formHelper->addVariable("UID:", ivar);
	uidDisplay->setTooltip("UID of the selected object.");
	uidDisplay->setEditable(false);
}

void GUI::createSlotDisplay() {
	slotWindow = formHelper->addWindow(Eigen::Vector2i(100, 20), "Selected Planet");

	slotButton = formHelper->addButton("Establish City", []() {});
	slotWindow->setVisible(false);

}

void GUI::createTradeDisplay() {
	slotWindow = formHelper->addWindow(Eigen::Vector2i(100, 100), "Trade Deal");
	slotButton = formHelper->addButton("Establish Trade", []() {});
}

void GUI::unselectSelection(Client* client, std::vector<GameObject*>& old_selection) {
	/* For now, let's just unselect() all selectables */
	for (GameObject* single_object : old_selection) {
		Selectable* selectable_object = dynamic_cast<Selectable*>(single_object);
		if (selectable_object) {
			selectable_object->unselect(this, client);
		}
	}
}

void GUI::selectSelection(Client* client, std::vector<GameObject*>& new_selection) {
	/* For now, let's just select() all selectables */
	for (GameObject* single_object : new_selection) {
		Selectable* selectable_object = dynamic_cast<Selectable*>(single_object);
		if (selectable_object) {
			selectable_object->select(this, client);
		}

		uidDisplay->setValue(single_object->getID());
	}
}

void GUI::createCityDisplay() {
	cityWindow = formHelper->addWindow(Eigen::Vector2i(100, 20), "Selected City");
	createUnitButton = formHelper->addButton("Create Unit", []() {});

	cityWindow->setVisible(false);

}

void GUI::displaySlotUI(Slot* slot, std::function<void()> createCityCallback) {
	slotWindow->setTitle(slot->getPlanet()->getName());
	slotButton->setCallback([createCityCallback, this]() {
		createCityCallback();
		slotWindow->setVisible(false);
	});

	slotWindow->center();
	slotWindow->setVisible(true);
}

void GUI::hideSlotUI() {
	slotWindow->setVisible(false);
}

void GUI::displayCityUI(City* city, std::function<void()> unitCreateCallback) {
	cityWindow->setTitle(city->get_slot()->getPlanet()->getName());
	createUnitButton->setCallback(unitCreateCallback);

	cityWindow->center();
	cityWindow->setVisible(true);
}

void GUI::hideCityUI() {
	cityWindow->setVisible(false);
}
