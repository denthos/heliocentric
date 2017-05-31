#pragma once

#include <nanogui\nanogui.h>
#include "unit_create_button.h"
#include "unit_spawner.h"
using namespace nanogui;

class UnitSpawnWidget : public Widget {
public:
	UnitSpawnWidget(Widget* parent);

	void updateSelection(UnitSpawner* spawner);
	void setCreateButtonCallback(std::function<void(UnitType*)> callback);

private:
	void createButtons();
	void createProgressbar();

	std::vector<UnitCreateButton*> createUnitButtons;
	ProgressBar* createUnitProgress;
};