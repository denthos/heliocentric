#include "unit_spawn_widget.h"
#include "unit_type.h"

UnitSpawnWidget::UnitSpawnWidget(Widget* parent) : Widget(parent) {
	this->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill));

	createProductionLabel();
	createButtons();
	createProgressbar();
	createQueueLabel();
}

void UnitSpawnWidget::createButtons() {
	for (int i = UnitType::FIRST; i < UnitType::NUM_TYPES; i++) {
		UnitType::TypeIdentifier ident = static_cast<UnitType::TypeIdentifier>(i);
		UnitType* type = UnitType::getByIdentifier(ident);

		createUnitButtons.push_back(new UnitCreateButton(this, type));
	}

	for (int i = BuildingType::FIRST; i < BuildingType::NUM_TYPES; i++) {
		BuildingType::TypeIdentifier ident = static_cast<BuildingType::TypeIdentifier>(i);
		BuildingType* type = BuildingType::getByIdentifier(ident);

		createBuildingButtons.push_back(new BuildingCreateButton(this, type));
	}
}

void UnitSpawnWidget::createProgressbar() {
	createUnitProgress = new ProgressBar(this);
	createUnitProgress->setValue(0.0f);

}

void UnitSpawnWidget::createQueueLabel() {
	queueLabel = new Label(this, "In Queue: 999");
}

void UnitSpawnWidget::createProductionLabel() {
	productionLabel = new Label(this, "Production: 999");
}

void UnitSpawnWidget::updateSelection(Builder* spawner, Player* player) {

	for (auto& button : createUnitButtons) {
		button->updateCreateButton(player);
	}

	for (auto& button : createBuildingButtons) {
		button->updateCreateButton(player);
	}

	if (spawner->isProducing()) {
		createUnitProgress->setValue((float)spawner->getPercentCompletion() / 100.0f);
	}
	else {
		createUnitProgress->setValue(0.0f);
	}

	productionLabel->setCaption("Production: " + std::to_string(spawner->get_production()));
	queueLabel->setCaption("In Queue: " + std::to_string(spawner->getProductionQueue().size()));
}

void UnitSpawnWidget::setCreateButtonCallback(std::function<void(Buildable*)> callback) {
	for (auto& button : createUnitButtons) {
		button->setCallback(callback);
	}

	for (auto& button : createBuildingButtons) {
		button->setCallback(callback);
	}
}
