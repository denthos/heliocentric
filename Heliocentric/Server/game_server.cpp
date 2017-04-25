#include "game_server.h"

GameServer::GameServer(int tick_duration, std::string port, int listen_queue, int poll_timeout) : SunNet::ChanneledServer<SunNet::TCPSocketConnection>("0.0.0.0", port, listen_queue, poll_timeout) {
	game_running = true;
	this->tick_duration = tick_duration;
}

GameServer::~GameServer() {

}

void GameServer::run() {
	std::clock_t tick_start_time;
	std::clock_t tick_elapsed_time;
	while (game_running) {
		tick_start_time = std::clock();

		// Add server logic

		// Uncomment to force each tick run longer than expected.
		//while (std::clock() - start_time < 2 * tick_duration);

		while ((tick_elapsed_time = std::clock() - tick_start_time) < tick_duration);
		if (tick_elapsed_time > tick_duration)
			Lib::LOG_WARN("Tick processing took ", tick_elapsed_time - tick_duration, "ms longer than expected");
	}
	Lib::LOG_DEBUG("Server exited safely");
}

void GameServer::end_game() {
	game_running = false;
}