#pragma once
#include <nanogui\nanogui.h>
using namespace nanogui;

#include "unit_type.h"
#include "resources.h"

class UnitCreateButton : public Button {
public:
	UnitCreateButton(Widget* parent, UnitType* unit_type);
	void updateCreateButton(Player* player);
	void setCallback(std::function<void(UnitType*)> callback);

private:
	UnitType* type;
};