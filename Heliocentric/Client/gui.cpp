#include "gui.h"
#include "slot.h"
#include "planet.h"
#include "selectable.h"
#include "city.h"
#include <iostream>
using namespace nanogui;

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

	// TODO: Make this a max of 16 characters
	cityNameDisplay = formHelper->addVariable("Name", cityName);

	for (int resource_type = Resources::FIRST; resource_type != Resources::NUM_RESOURCES; resource_type++) {
		Resources::Type type_of_resource = static_cast<Resources::Type>(resource_type);
		int rvar = 0;
		resourceDisplay.insert(std::make_pair(type_of_resource, formHelper->addVariable(Resources::toString(type_of_resource), rvar)));
	}

	slotButton = formHelper->addButton("Establish City", []() {});
	slotWindow->setVisible(false);
}

void GUI::createTradeDisplay() {
	tradeWindow = formHelper->addWindow(Eigen::Vector2i(500, 500), "Trade Deal");
	createTradeButton = formHelper->addButton("Establish Trade", []() {});
	//ComboBox* listOfPlayers = new ComboBox(tradeWindow, { "player 1", "player 2", "player 3" });
	formHelper->addGroup("Product to trade: ");
	formHelper->addVariable("Resource type", Resources::ALUMINUM, true)->setItems(Resources::nameMap);
	//formHelper->addVariable("string", "Select Player", enabled);
	tradeWindow->setVisible(true);
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

void GUI::displaySlotUI(Slot* slot, std::function<void(std::string)> createCityCallback) {
	slotWindow->setTitle(slot->getPlanet()->getName());
	slotButton->setCallback([createCityCallback, this]() {
		createCityCallback(cityName);
		slotWindow->setVisible(false);
	});

	for (int resource_type = Resources::FIRST; resource_type != Resources::NUM_RESOURCES; resource_type++) {
		Resources::Type type_of_resource = static_cast<Resources::Type>(resource_type);
		int resource_count = slot->getResourceCount(type_of_resource);
		resourceDisplay[type_of_resource]->setValue(resource_count);
	}

	slotWindow->center();
	slotWindow->setVisible(true);
}
/*
void GUI::displayTradeUI(Player* player, std::function<void()> createTrade) {
	tradeWindow->setTitle("Trade deal with " + player->get_name());
	createTradeButton->setCallback([createTrade, this]() {
		createTrade();
	});
}*/

void GUI::hideSlotUI() {
	slotWindow->setVisible(false);
}

void GUI::displayCityUI(City* city, std::function<void()> unitCreateCallback) {
	cityWindow->setTitle(city->getName());
	createUnitButton->setCallback(unitCreateCallback);

	cityWindow->center();
	cityWindow->setVisible(true);
}

void GUI::hideCityUI() {
	cityWindow->setVisible(false);
}
