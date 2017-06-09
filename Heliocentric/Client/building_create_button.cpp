#include "building_create_button.h"

BuildingCreateButton::BuildingCreateButton(Widget* parent, BuildingType* type) :
	Button(parent), type(type) {

	this->setCaption(type->getTypeName());
	std::ostringstream infoDisplay;

	infoDisplay << type->getTypeName() << std::endl;
	for (auto& resource_pair : type->getBuildRequirements()) {
		infoDisplay << Resources::toString(resource_pair.first) << ": " << resource_pair.second << std::endl;
	}

	this->setTooltip(infoDisplay.str());
}

void BuildingCreateButton::updateCreateButton(Player* player) {
	/*
	Your cadaverous pallor betrays an aura of foreboding, almost as though you sense 
	a disquieting metamorphosis. Is this haunted room actually stretching? Or is it 
	your imagination, hmm? And consider this dismaying observation: This chamber has 
	no windows and no doors, which offers you this chilling challenge: to find a way 
	out!
	*/
	this->setEnabled(type->hasBuildRequirements(player->getResources()));
}

void BuildingCreateButton::setCallback(std::function<void(BuildingType*)> callback) {
	Button::setCallback([this, callback]() {
		this->setEnabled(false);
		this->setTooltip("Can only construct one " + type->getTypeName());
		callback(this->type);
	}); 
}