#pragma once

#include <nanogui\nanogui.h>
#include "resources.h"
#include "unit_create_button.h"
#include "unit_spawner.h"
using namespace nanogui;

class UnitSpawnWidget : public Widget {
public:
	UnitSpawnWidget(Widget* parent);

	void updateSelection(UnitSpawner* spawner, Player* player);
	void setCreateButtonCallback(std::function<void(UnitType*)> callback);

private:
	void createButtons();
	void createProgressbar();
	void createProductionLabel();
	void createQueueLabel();

	Label* productionLabel;

	std::vector<UnitCreateButton*> createUnitButtons;
	ProgressBar* createUnitProgress;
	Label* queueLabel;
};