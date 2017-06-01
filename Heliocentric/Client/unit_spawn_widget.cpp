#include "unit_spawn_widget.h"
#include "unit_type.h"

UnitSpawnWidget::UnitSpawnWidget(Widget* parent) : Widget(parent) {
	this->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill));

	createButtons();
	createProgressbar();
}

void UnitSpawnWidget::createButtons() {
	for (int i = UnitType::FIRST; i < UnitType::NUM_TYPES; i++) {
		UnitType::TypeIdentifier ident = static_cast<UnitType::TypeIdentifier>(i);
		UnitType* type = UnitType::getByIdentifier(ident);

		createUnitButtons.push_back(new UnitCreateButton(this, type));
	}
}

void UnitSpawnWidget::createProgressbar() {
	createUnitProgress = new ProgressBar(this);
	createUnitProgress->setValue(0.0f);

	queueLabel = new Label(this, "In Queue: 999");
}

void UnitSpawnWidget::updateSelection(UnitSpawner* spawner) {
	if (spawner->isProducing()) {
		createUnitProgress->setValue((float)spawner->getPercentCompletion() / 100.0f);
	}
	else {
		createUnitProgress->setValue(0.0f);
	}

	queueLabel->setCaption("In Queue: " + std::to_string(spawner->getUnitSpawnQueue().size()));
}

void UnitSpawnWidget::setCreateButtonCallback(std::function<void(UnitType*)> callback) {
	for (auto& button : createUnitButtons) {
		button->setCallback(callback);
	}
}