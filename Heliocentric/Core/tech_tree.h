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
	int id;
	std::string description;
	std::string name;
	bool researched;
	float research_points_required;
	float research_points_accumulated;
	float research_progress; // percentage of research points accumulated
	std::vector<Technology*> parents;
	std::vector<Technology*> children;

public:

	int getID() const;
	std::string getName() const;
	std::string getDescription() const;
	const std::vector<Technology*>& getChildren() const;

	Technology(int, float, std::string name, std::string desc);

	/**
	Called by tech tree, which delegates funtionalities to this method.
	@param research_points Reserch points being accumulated.
	*/
	void research(float);

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
	/* TODO: Potential race condition? Might need a lock on current_research. */
	Technology* current_research; // The selected tech that is being researched
	static const std::unordered_map<int, std::string> tech_name_map;
	std::vector<Technology*> available_techs;

	void build_tree();
	void set_research_idle();
	std::unordered_map<int, Technology*> techs;

public:
	const Technology* getTechById(int id) const;
	TechTree();
	~TechTree();

	/**
	Called every server tick to accumulate research points on a selected tech.
	@param research_points Reserch points being accumulated each time this method is called.
	@return False if no research is selected. True otherwise.
	*/
	bool TechTree::research(float research_points, Technology*& current);

	/**
	Chooses a tech as currently researching tech.
	@param tech ID of the tech to choose.
	*/
	void choose_tech(int);

	/**
	Returns ID of current research.
	@return ID of current research.
	*/
	int get_current_research_id();

	/**
	Returns name of current research.
	@return name of current research.
	*/
	std::string get_current_research_name();

	/**
	Returns progress of current research.
	@return progress of current research.
	*/
	float get_current_research_progress();

	bool is_researching();

	/**
	Gets all the available techs, which are techs with all prerequisites satisfied and
	have not been researched yet.
	@return Vector of IDs of techs that can be researched.
	*/
	std::vector<int> get_available_techs();

	class BadTechIDException : std::exception {};
	class ResearchIdleException : std::exception {};
};
