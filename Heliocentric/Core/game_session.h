#pragma once
#include "player.h"

class GameServer;

/**
Instance of a game. Keeps track of all information about current game settings.
Deleted on server after a game has ended.

Each in-game day is one second in real life.
*/
class GameSession {
friend class GameServer;

private:
	int time_limit; // Time limit in days/seconds.
	float time_elapsed; // Elapsed time in seconds.

	bool game_end;
	UID winner;

	// Enabled victory conditions
	bool time_victory;
	bool domination_victory;
	bool economic_victory;
	bool science_victory;

public:
	GameSession();

	/**
	Called after each server tick to increment elapsed time in game.
	@param tick_duration Duration of each tick in ms.
	*/
	void increment_time(int);

	/**
	Returns how many days have passed in game.
	@return Days passed in game.
	*/
	int days_passed();

	/**
	Tells if time limit has reached.
	@return True if time limit has reached, false otherwise.
	*/
	bool time_limit_reached();
};
