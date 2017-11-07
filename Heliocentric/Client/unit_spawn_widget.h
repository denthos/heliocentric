#pragma once

#include <nanogui\nanogui.h>
#include "resources.h"
#include "unit_create_button.h"
#include "building_create_button.h"
#include "builder.h"
#include "buildable.h"
using namespace nanogui;

class UnitSpawnWidget : public Widget {
public:
	UnitSpawnWidget(Widget* parent);

	void updateSelection(Builder* spawner, Player* player);
	void setCreateButtonCallback(std::function<void(Buildable*)> callback);

private:
	void createButtons();
	void createProgressbar();
	void createProductionLabel();
	void createQueueLabel();

	Label* productionLabel;

	std::vector<UnitCreateButton*> createUnitButtons;
	std::vector<BuildingCreateButton*> createBuildingButtons;
	ProgressBar* createUnitProgress;
	Label* queueLabel;
};