#include "game_session.h"

GameSession::GameSession() : time_limit(900), time_elapsed(0), game_running(true), winner(INVALID_ID), time_victory(true),
domination_victory(false), economic_victory(false), science_victory(false) {

}

void GameSession::increment_time(int tick_duration) {
	time_elapsed += tick_duration / 1000.0f;
}

int GameSession::days_passed() {
	return (int)time_elapsed;
}

bool GameSession::time_limit_reached() {
	return days_passed() > time_limit;
}
