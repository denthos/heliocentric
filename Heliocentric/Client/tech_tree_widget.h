#pragma once
#include <nanogui\nanogui.h>
using namespace nanogui;
#include "tech_tree.h"
#include "technology_button.h"


class TechTreeWidget : public Widget {
public:
	TechTreeWidget(Widget* parent, std::function<void(const Technology*)> techButtonPressedCallback);
	virtual ~TechTreeWidget();

	void updateTechTreeWidget(TechTree* tree);
	void handleTechnologyButtonPress(TechnologyButton* tech);

private:
	/* 
	NanoGUI crashes with an "integer divide by zero" err if we remove all 
	buttons from the widget (via removechild). [The crash happens next performLayout()]

	To overcome this, we always have this label in the widget and just make it invisible
	until the right time
	*/
	Label* doneResearchingLabel;

	/* 
	We want to make sure that we only have one button for each tech..
	We keep these pointers as nanogui::ref so that they are not de-allocated when
	removeChild() is called with them as the parameter (why does nanogui de-allocate stuff
	that we allocated? :/)

	Part of the ship, part of the crew.
	*/
	std::unordered_map<int, ref<TechnologyButton>> visibleTechButtons;
	std::unordered_map<int, ref<TechnologyButton>> hiddenTechButtons;

	void addTechButton(const Technology*);

	std::function<void(const Technology*)> technologyPressedCallback;
};