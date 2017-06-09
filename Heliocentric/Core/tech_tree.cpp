#include "tech_tree.h"
#include "logging.h"


Technology::Technology(int tech, float research_points_required, std::string name, std::string desc) : 
	id(tech), name(name), description(desc), researched(false), research_points_required(research_points_required),
	available(false), prereq_met(false) {
}

void Technology::research(float research_points) {
	research_points_accumulated += research_points;
	research_progress = 100 * (research_points_accumulated / research_points_required);
	researched = research_points_accumulated >= research_points_required;

	/* Set this tech to unavailable  */
	if (researched) {
		available = false;
	}
}

bool Technology::is_available() {
	/* Check prerequisites if not already met. */
	if (!prereq_met) {
		/* Not available if any of its parents has not been researched yet. */
		for (auto it : parents) {
			if (!it->researched) {
				return false;
			}
		}

		/* If all prereq checks passed, set this boolean to true and make this tech available. */
		prereq_met = true;
		available = true;
	}

	return available;
}


bool Technology::hasResearched() const {
	return researched;
}

int Technology::getID() const {
	return id;
}

std::string Technology::getName() const {
	return name;
}

std::string Technology::getDescription() const {
	return description;
}

const std::vector<Technology*>& Technology::getChildren() const {
	return children;
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

const Technology* TechTree::getTechById(int id) const {
	auto& tech_it = techs.find(id);
	if (tech_it == techs.end()) {
		return nullptr;
	}
	else {
		return tech_it->second;
	}
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
	techs[TECH_1] = new Technology(TECH_1, 100.0f, "Steel Plating", "New Units are Spawned with More Defense");
	techs[TECH_2] = new Technology(TECH_2, 100.0f, "Advanced Mining", "Provides 25% chance of extracting additional resources from each occupied slot.");
	techs[TECH_3] = new Technology(TECH_3, 400.0f, "Heavy Unit", "Allows Spawning of a Heavy Unit");
	techs[TECH_4] = new Technology(TECH_4, 400.0f, "Tech 4", "Does a tech");
	techs[TECH_5] = new Technology(TECH_5, 400.0f, "Exploration Program", "Increases the city settlement limit (+1).");
	techs[TECH_6] = new Technology(TECH_6, 800.0f, "Tech 6", "Does a tech");

	techs[TECH_1]->add_children({ techs[TECH_3], techs[TECH_4] });
	techs[TECH_2]->add_children({ techs[TECH_5] });
	techs[TECH_3]->add_children({ techs[TECH_6] });
	techs[TECH_4]->add_children({ techs[TECH_6] });
	techs[TECH_5]->add_children({ techs[TECH_6] });
}

void TechTree::set_research_idle() {
	current_research = nullptr;
}

bool TechTree::research(float research_points, Technology*& current) {
	if (current_research == nullptr) {
		/* Maybe somehow notify the player that they don't have a tech selected instead of spamming the console */
		return false;
	}
	else {
		current_research->research(research_points);

		current = current_research;

		if (current_research->researched) {
			LOG_INFO(current_research->name + " is unlocked");
			set_research_idle();
		}
	}

	return true;
}

void TechTree::choose_tech(int tech) {
	if (techs.find(tech) == techs.end()) {
		LOG_ERR("Woah! A nonexistent tech was selected: ", tech);
		throw BadTechIDException();
	}

	current_research = techs[tech];
	LOG_DEBUG("Now researching ", current_research->name);
}

bool TechTree::is_researching() {
	return (current_research != nullptr);
}

int TechTree::get_current_research_id() {
	if (!current_research) {
		throw ResearchIdleException();
	}
	return current_research->id;
}

std::string TechTree::get_current_research_name() {
	if (!current_research) {
		throw ResearchIdleException();
	}
	return current_research->name;
}

float TechTree::get_current_research_progress() {
	if (!current_research) {
		throw ResearchIdleException();
	}
	return current_research->research_progress;
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
