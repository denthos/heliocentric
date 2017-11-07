#include "unit.h"
#include "unit_type.h"
#include "attackable_gameobject_widget.h"

AttackableGameObjectWidget::AttackableGameObjectWidget(Widget* parent) : Widget(parent)
{

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
}

AttackableGameObjectWidget::~AttackableGameObjectWidget()
{
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

void AttackableGameObjectWidget::updateSelection(AttackableGameObject * selected)
{
	Attack selected_attack = selected->getAttack();
	attack_strength->setValue((int)selected_attack.getDamage());
	attack_range->setValue((int)selected_attack.getRange()); 
	combat_defense_strength->setValue(selected->get_combat_defense());
	health_stat->setCaption( std::to_string(selected->get_health())+"%");
	healthbar->setValue((float)(selected->get_health())/100.0f);

	player_name->setCaption(selected->get_player()->get_name());
	glm::vec3 rgbVec = PlayerColor::colorToRGBVec(selected->get_player()->getColor());
	player_name->setColor(Color(Eigen::Vector3f(rgbVec.x, rgbVec.y, rgbVec.z)));

	float health;
	int health_percentage;

	Unit* unit = dynamic_cast<Unit*>(selected);
	if (unit) {
		health = ((float)unit->get_health() / (float)unit->getType()->getBaseHealth());
		health_percentage = (int)(health * 100.0f);
	}
	else {
		health = ((float)selected->get_health() / 800.0f);
		health_percentage = (int)(health * 100.0f);
	}

	health_stat->setCaption(std::to_string(health_percentage) + "%");
	healthbar->setValue(health);
}

