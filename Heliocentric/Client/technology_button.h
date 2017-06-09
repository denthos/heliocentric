#pragma once
#include <nanogui\nanogui.h>
#include "tech_tree.h"

using namespace nanogui;

class TechnologyButton : public Button {
public:
	TechnologyButton(Widget* parent, const Technology* tech);
	virtual ~TechnologyButton();

	void setCallback(std::function<void(TechnologyButton*)> callback);

	const Technology* getTech() const;

private:
	const Technology* tech;
};