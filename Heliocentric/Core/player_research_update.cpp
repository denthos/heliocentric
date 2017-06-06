#include "player_research_update.h"

PlayerResearchUpdate::PlayerResearchUpdate(UID id, int tech_id, float research_points) :
	id(id), tech_id(tech_id), research_points(research_points) {

}

void PlayerResearchUpdate::apply(Player* player) {
	player->choose_research(tech_id);
	player->research(research_points);
}