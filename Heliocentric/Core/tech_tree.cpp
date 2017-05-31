#include "tech_tree.h"
#include "logging.h"

/* TODO: Should load from config file */
std::unordered_map<int, std::string> tech_name_map = {
	{1, "Tech 1"},
	{2, "Tech 2"},
	{3, "Tech 3"},
	{4, "Tech 4"},
	{5, "Tech 5"},
	{6, "Tech 6"}
};

Technology::Technology(int tech, float research_points_required) :
	researched(false), research_points_required(research_points_required) {
	this->name = tech_name_map[tech];
}

void Technology::research(float research_points) {
	research_points_accumulated += research_points;
	research_progress = 100 * (research_points_accumulated / research_points_required);
	researched = research_points_accumulated >= research_points_required;
}

bool Technology::is_available() {
	for (auto it : parents) {
		if (!it->researched) {
			return false;
		}
	}

	return true;
}

void Technology::add_children(std::vector<Technology*> children) {
	for (auto it : children) {
		this->children.push_back(it);
		it->parents.push_back(this);
	}
}


TechTree::TechTree() {
	build_tree();
	set_research_idle();
}

TechTree::~TechTree() {

}

void TechTree::build_tree() {
	/* Manually build the tree (I know it looks ugly) */
	/**
	tech 1 leads to tech 3 and 4
	tech 2 leads to tech 5
	tech 3, 4 and 5 leads to tech 6
	*/
	techs[TECH_1] = new Technology(TECH_1, 100.0f);
	techs[TECH_2] = new Technology(TECH_2, 100.0f);
	techs[TECH_3] = new Technology(TECH_3, 400.0f);
	techs[TECH_4] = new Technology(TECH_4, 400.0f);
	techs[TECH_5] = new Technology(TECH_5, 400.0f);
	techs[TECH_6] = new Technology(TECH_6, 800.0f);

	techs[TECH_1]->add_children({ techs[TECH_3], techs[TECH_4] });
	techs[TECH_2]->add_children({ techs[TECH_5] });
	techs[TECH_3]->add_children({ techs[TECH_6] });
	techs[TECH_4]->add_children({ techs[TECH_6] });
	techs[TECH_5]->add_children({ techs[TECH_6] });
}

void TechTree::set_research_idle() {
	current_research = nullptr;
}

void TechTree::research(float research_points) {
	if (current_research == nullptr) {
		/* Maybe somehow notify the player that they don't have a tech selected instead of spamming the console */
		return;
	}
	else {
		current_research->research(research_points);

		if (current_research->researched) {
			LOG_DEBUG(current_research->name + " is unlocked");
			set_research_idle();
		}
	}
}

void TechTree::choose_tech(int tech) {
	if (techs.find(tech) == techs.end()) {
		LOG_ERR("Woah! A nonexistent tech was selected!");
		throw BadTechIDException();
	}

	current_research = techs[tech];
	LOG_INFO("Now researching ", current_research->name);
}

std::vector<int> TechTree::get_available_techs() {
	/* TODO: Implement a more efficient algorithm to find all available techs */
	std::vector<int> ret;
	for (auto it : techs) {
		if (it.second->is_available()) {
			ret.push_back(it.first);
		}
	}

	return ret;
}
