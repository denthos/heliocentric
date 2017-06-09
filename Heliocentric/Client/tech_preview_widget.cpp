#include "tech_preview_widget.h"
#include "tech_tree.h"

TechPreviewWidget::TechPreviewWidget(Widget* parent, std::string font, int font_size, std::function<void()> chooseTechButtonCallback) : 
	Widget(parent), chooseTechButtonCallback(chooseTechButtonCallback), font(font), font_size(font_size) {
	this->setLayout(new GridLayout(Orientation::Horizontal, 1, Alignment::Fill));
	this->setWidth(150);
	this->setHeight(500);

	createCurrentTechLabel();
	createProgressBar();
	createChooseButton(chooseTechButtonCallback);
}

void TechPreviewWidget::createCurrentTechLabel() {
	currentTechLabel = new Label(this, "Researching:DB PROGRAMMING", font, font_size);
}

void TechPreviewWidget::createProgressBar() {
	currentResearchProgressBar = new ProgressBar(this);
	currentResearchProgressBar->setValue(0.0f);
}

void TechPreviewWidget::createChooseButton(std::function<void()> callback) {
	chooseTechButton = new Button(this, "Choose Tech");
	chooseTechButton->setCallback(callback);
}

void TechPreviewWidget::updatePreview(TechTree* tree) {
	// TODO: What if we are not researching anything?
	std::string current_research = "";
	float current_progress = 0.0f;
	try {
		current_research = tree->get_current_research_name();
		current_progress = tree->get_current_research_progress() / 100.0f;
	}
	catch (const TechTree::ResearchIdleException&) {}

	this->currentTechLabel->setCaption("Researching: " + current_research);
	this->currentResearchProgressBar->setValue(current_progress);
}