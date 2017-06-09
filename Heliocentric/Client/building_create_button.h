#pragma once
#include <nanogui\nanogui.h>
using namespace nanogui;

#include "building_type.h"
#include "resources.h"
#include "player.h"

class BuildingCreateButton : public Button {
public:
	BuildingCreateButton(Widget* parent, BuildingType* building_type);
	void updateCreateButton(Player* player);
	void setCallback(std::function<void(BuildingType*)> callback);
private:
	BuildingType* type;
};