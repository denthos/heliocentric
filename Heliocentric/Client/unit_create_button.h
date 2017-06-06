#pragma once
#include <nanogui\nanogui.h>
using namespace nanogui;

#include "unit_type.h"

class UnitCreateButton : public Button {
public:
	UnitCreateButton(Widget* parent, UnitType* unit_type);
	void setCallback(std::function<void(UnitType*)> callback);

private:
	UnitType* type;
};