#pragma once
#include <nanogui\nanogui.h>
using namespace nanogui;

class TechTree;

class TechPreviewWidget : public Widget {
public:
	TechPreviewWidget(Widget* parent, std::function<void()> chooseTechButtonCallback);
	void updatePreview(TechTree* tree);

private:
	void createCurrentTechLabel();
	void createProgressBar();
	void createChooseButton(std::function<void()> callback);

	Label* currentTechLabel;
	ProgressBar* currentResearchProgressBar;

	Button* chooseTechButton;
	std::function<void()> chooseTechButtonCallback;
};