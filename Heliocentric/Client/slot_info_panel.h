#pragma once
#include "resources.h"
#include "slot.h"

#include <unordered_map>
#include <nanogui/nanogui.h>

using namespace nanogui;

/* The panel that contains all information pertaining to a slot */
class SlotInfoPanel : public Widget {
public:
	SlotInfoPanel(Widget* parent);
	virtual ~SlotInfoPanel();

	void updateDisplay(const Slot* slot);

private:

	/* Data members */
	Label* planetLabel;
	Label* planetName;

	std::unordered_map<Resources::Type, std::pair<Label*, IntBox<int>*>> resourceDisplay;

	/* Builder functions */
	void buildLayout();
	void buildPlanetDisplay();
	void buildResourceDisplay();
};