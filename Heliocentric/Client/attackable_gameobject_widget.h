
#pragma once
#include "gui_utilities.h"
#include "attackable_game_object.h"
using namespace nanogui;

class AttackableGameObjectWidget : public Widget {
public:
	AttackableGameObjectWidget(Widget* parent);
	~AttackableGameObjectWidget();
	virtual void updateSelection(AttackableGameObject * selected);
private:
	Widget* info_widget;
	//combatdefense
	Label* combat_defense_label;
	IntBox<int>* combat_defense_strength;

	//attack
	Label* attack_label;
	IntBox<int>* attack_strength;
	Label* attack_range_label;
	IntBox<int>* attack_range;

	//health
	Label* health_label;
	Label* health_stat;
	ProgressBar* healthbar;

	//playername
	Label* player_label;
	Label* player_name;
};