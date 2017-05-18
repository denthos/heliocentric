
#pragma once
#include "gui.h"
#include "attackable_game_object.h"
class UnitGUI : public GUI {
public:
	UnitGUI(GLFWwindow *);
	~UnitGUI();
	virtual void updateSelection(GameObject * selected);
private:
	detail::FormWidget<int> * uidDisplay;

	Widget* close_widget;
	Button* close_button;

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