#include "unit_create_button.h"

UnitCreateButton::UnitCreateButton(Widget* parent, UnitType* type) :
	Button(parent), type(type) {

	this->setCaption(type->getTypeName());

	std::ostringstream infoDisplay;
	infoDisplay << type->getTypeName() << std::endl;
	for (auto& resource_pair : type->getBuildRequirements()) {
		infoDisplay << Resources::toString(resource_pair.first) << ": " << resource_pair.second << std::endl;
	}

	// TODO: make this tooltip display instantaneous and pretteir
	this->setTooltip(infoDisplay.str());
}

void UnitCreateButton::setCallback(std::function<void(UnitType*)> callback) {
	Button::setCallback(std::bind(callback, this->type));
}
