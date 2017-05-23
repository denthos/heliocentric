#include "unit_window.h"

UnitWindow::UnitWindow(Widget* parent, const std::string &title) : Window(parent, title)
{

	this->setPosition(Vector2i(10 , 40));

	setLayout(new GroupLayout());

	//unit info
	info_widget = new Widget(this);
	info_widget->setLayout(new GridLayout(Orientation::Horizontal, 2, Alignment::Fill, 0, 0));

	//player
	player_label = new Label(info_widget, "Player: "); 
	player_name = new Label(info_widget, "Geoff Voelker");

	//attack
	attack_label = new Label(info_widget, "Attack Damage: ");
	attack_strength = new IntBox<int>(info_widget);
	attack_strength->setEditable(false);

	attack_range_label = new Label(info_widget, "Attack Range: ");
	attack_range = new IntBox<int>(info_widget);
	attack_range->setEditable(false);

	//defense
	combat_defense_label = new Label(info_widget, "Combat Defense: ");
	combat_defense_strength = new IntBox<int>(info_widget);
	combat_defense_strength->setEditable(false);

	//health bar
	health_label = new Label(info_widget, "Health: ");
	health_stat = new Label(info_widget, "100%");

	healthbar = new ProgressBar(this);
	healthbar->setValue(1.0f);

	//close button
	close_widget = new Widget(this);
	close_widget->setLayout(new BoxLayout(Orientation::Vertical, Alignment::Middle, 0, 0));

	close_button = new Button(close_widget, "close");
	close_button->setCallback([&] {
		this->setVisible(false);
	});


	setVisible(false);
	
}UnitWindow::~UnitWindow()
{
	//delete close_widget;
	delete close_button;

	//delete info_widget;
	
	delete combat_defense_label;
	delete combat_defense_strength;

	//attack
	delete attack_label;
	delete attack_strength;
	delete attack_range_label;
	delete attack_range;

	//health
	delete health_label;
	delete health_stat;
	delete healthbar;

	//playername
	delete player_label;
	delete player_name;
}

void UnitWindow::updateSelection(AttackableGameObject * selected)
{
	AttackableGameObject* unit = dynamic_cast<AttackableGameObject*>(selected);

	if (unit) {
		Attack unit_attack = unit->getAttack();
		attack_strength->setValue(unit_attack.getDamage());
		attack_range->setValue(unit_attack.getRange()); 
		combat_defense_strength->setValue(unit->get_combat_defense());
		health_stat->setCaption( std::to_string(unit->get_health())+"%");
		healthbar->setValue((float)(unit->get_health())/100.0f);
		player_name->setCaption(unit->get_player()->get_name());

	}
}
