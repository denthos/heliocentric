#include "tech_tree_widget.h"
#include "tech_tree.h"
#include <unordered_set>

TechTreeWidget::TechTreeWidget(Widget* parent, std::function<void(const Technology*)> techButtonPressCallback) :
	Widget(parent), technologyPressedCallback(techButtonPressCallback) {
	// Let's calculate the width as the number of nodes per level
	this->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill));

	/* 
	TODO: Create a button for each technology here. This would be the correct way to
	do things, but unfortunately, the game's Tech Tree is not statically defined :(

	Too late to fix this now. Let's just populate things dynamically instead.
	*/
	doneResearchingLabel = new Label(this, "No more techs to research.");
	doneResearchingLabel->setVisible(false);
}


TechTreeWidget::~TechTreeWidget() {
	for (auto& button : this->visibleTechButtons) {
		delete button.second;
	}

	for (auto& button : this->hiddenTechButtons) {
		delete button.second;
	}
}

void TechTreeWidget::updateTechTreeWidget(TechTree* tree) {
	/*
	At this point, we need to do the following:
	2. Make sure that all available researches are displayed
	3. Make sure that the thing currently being researched is disabled
	*/
	this->setSize(Eigen::Vector2i(250, 500));

	/* First, map all the available techs (I wish this was in tech_tree or smth) */
	std::unordered_map<int, const Technology*> available_techs;
	for (int i : tree->get_available_techs()) {
		const Technology* tech = tree->getTechById(i);
		if (!tech) {
			continue;
		}
		available_techs.insert(std::make_pair(tech->getID(), tech));
	}

	if (available_techs.size() == 0) {
		doneResearchingLabel->setVisible(true);
	}

	/* Now remove all that is visible that shouldn't be */
	for (auto visible_it = this->visibleTechButtons.begin(); visible_it != this->visibleTechButtons.end();) {
		bool is_availble = (available_techs.find(visible_it->first) != available_techs.end());
		if (!is_availble) {
			hiddenTechButtons.insert(std::make_pair(visible_it->first, visible_it->second));
			this->removeChild(visible_it->second.get());
			visible_it = this->visibleTechButtons.erase(visible_it);
		}
		else {
			visible_it++;
		}
	}

	/* Now make all that is available visible */
	for (auto& available_it : available_techs) {
		this->addTechButton(available_it.second);
	}

	/* Now we make our current research disabled */
	try {
		int current_research = tree->get_current_research_id();
		if (visibleTechButtons.find(current_research) == visibleTechButtons.end()) {
			return;
		}

		visibleTechButtons[current_research]->setEnabled(false);
	}
	catch (const TechTree::ResearchIdleException&) {}

}

void TechTreeWidget::addTechButton(const Technology* tech) {
	/* First, see if its already added */
	auto& butt_it = visibleTechButtons.find(tech->getID());
	if (butt_it != visibleTechButtons.end()) {
		butt_it->second->setEnabled(true);
		return;
	}

	/* Now see if it is invisible and simply needs to be moved */
	butt_it = hiddenTechButtons.find(tech->getID());
	if (butt_it != hiddenTechButtons.end()) {
		// Move it
		this->addChild(butt_it->second);
		visibleTechButtons.insert(std::make_pair(butt_it->first, butt_it->second));
		hiddenTechButtons.erase(butt_it->first);
	}
	else {
		// Make a new one
		TechnologyButton* button = new TechnologyButton(this, tech);
		button->setCallback(std::bind(&TechTreeWidget::handleTechnologyButtonPress, this, std::placeholders::_1));
		this->visibleTechButtons.insert(std::make_pair(tech->getID(), button));
	}

}

void TechTreeWidget::handleTechnologyButtonPress(TechnologyButton* button) {
	/* 
	This should fire a research event to the server. The UI will be updated when the
	client receives some updates about this
	*/

	/* Call the callback */
	this->technologyPressedCallback(button->getTech());
}