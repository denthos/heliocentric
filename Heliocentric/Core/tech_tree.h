#pragma once
#include <exception>
#include <string>
#include <unordered_map>
#include <vector>

#define TECH_1 1
#define TECH_2 2
#define TECH_3 3
#define TECH_4 4
#define TECH_5 5
#define TECH_6 6

/* These should be moved to config file */
#define TECH_ADVANCED_NANOTECHNOLOGY "Advanced Nanotechnology"
#define TECH_MEGASTRUCTURE "Megastructure"
#define TECH_NUCLEAR_FUSION "Nuclear Fusion"
#define TECH_SPACE_HABITATION "Space Habitation"

class Technology {
friend class TechTree;

private:
	std::string name;
	bool researched;
	int science_to_unlock;
	std::vector<Technology*> parents;
	std::vector<Technology*> children;

public:
	Technology(int, int);

	/**
	Tells if this tech is available for research.
	@return True if this teach can be researched, false otherwise.
	*/
	bool is_available();

	/**
	Adds all techs in parameter as children of this tech,
	and add this tech to be parent of each tech in parameter
	@param children Vector of children of this tech.
	*/
	void add_children(std::vector<Technology*>);
};

class TechTree {
private:
	Technology* researching; // The selected tech that is being researched
	static const std::unordered_map<int, std::string> tech_name_map;
	std::unordered_map<int, Technology*> techs;
	std::vector<Technology*> available_techs;

	void build_tree();

public:
	TechTree();
	~TechTree();

	void research(int);
	void choose_tech(int);
	std::vector<int> get_available_techs();

	class BadTechNameException : std::exception {};
};
