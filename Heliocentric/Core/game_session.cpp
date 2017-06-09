#include "game_session.h"
#include "math.h"
#include "logging.h"

GameSession::GameSession() : time_limit(60), time_elapsed(0.00f), game_running(true), winner(INVALID_ID), time_victory(true),
domination_victory(false), economic_victory(false), science_victory(false) {
	timeUpdate = std::make_shared<TimeUpdate>(time_elapsed);
}

void GameSession::increment_time(int tick_duration) {
	int new_time = round(time_elapsed + tick_duration / 1000.00f);
	timeUpdate->time = time_limit - new_time;
	time_elapsed = time_elapsed + tick_duration / 1000.00f;
}

int GameSession::days_passed() {
	return round(time_elapsed);
}

bool GameSession::time_limit_reached() {
	return days_passed() > time_limit;
}
