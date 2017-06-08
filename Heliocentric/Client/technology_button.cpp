#include "technology_button.h"

TechnologyButton::TechnologyButton(Widget* parent, const Technology* tech) :
	Button(parent, tech->getName()), tech(tech) {

	this->setTooltip(tech->getDescription());

}

TechnologyButton::~TechnologyButton() {}

void TechnologyButton::setCallback(std::function<void(TechnologyButton*)> callback) {
	Button::setCallback(std::bind(callback, this));
}

const Technology* TechnologyButton::getTech() const {
	return tech;
}