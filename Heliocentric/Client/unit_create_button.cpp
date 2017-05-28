#include "unit_create_button.h"

UnitCreateButton::UnitCreateButton(Widget* parent, UnitType* type) :
	Button(parent), type(type) {

	this->setCaption(type->getTypeName());
}

void UnitCreateButton::setCallback(std::function<void(UnitType*)> callback) {
	Button::setCallback(std::bind(callback, this->type));
}