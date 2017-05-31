#include "slot_info_panel.h"
#include "planet.h"

SlotInfoPanel::SlotInfoPanel(Widget* parent) : Widget(parent) {
	this->buildLayout();
	this->buildPlanetDisplay();
	this->buildResourceDisplay();
}

SlotInfoPanel::~SlotInfoPanel() {
	delete this->planetLabel;
	delete this->planetName;

	for (int i = Resources::FIRST; i != Resources::NUM_RESOURCES; i++) {
		Resources::Type resource_type = static_cast<Resources::Type>(i);

		delete this->resourceDisplay[resource_type].first;
		delete this->resourceDisplay[resource_type].second;
	}
}

void SlotInfoPanel::updateDisplay(const Slot* slot) {
	this->planetName->setCaption(slot->getPlanet()->getName());

	for (int i = Resources::FIRST; i != Resources::NUM_RESOURCES; i++) {
		Resources::Type resource_type = static_cast<Resources::Type>(i);

		this->resourceDisplay[resource_type].second->setValue(slot->getResourceCount(resource_type));
	}
}

void SlotInfoPanel::buildLayout() {
	this->setLayout(new GridLayout(Orientation::Horizontal, 2, Alignment::Fill));
}

void SlotInfoPanel::buildPlanetDisplay() {
	this->planetLabel = new Label(this, "Planet");
	this->planetName = new Label(this, "Geoff Voelker");
}

void SlotInfoPanel::buildResourceDisplay() {
	for (int i = Resources::FIRST; i != Resources::NUM_RESOURCES; i++) {
		Resources::Type resource_type = static_cast<Resources::Type>(i);

		Label* resourceLabel = new Label(this, Resources::toString(resource_type));
		IntBox<int>* dataDisplay = new IntBox<int>(this, 999999999);
		dataDisplay->setEditable(false);

		auto resource_pair = std::make_pair(resourceLabel, dataDisplay);

		this->resourceDisplay.insert(std::make_pair(resource_type, resource_pair));
	}
}