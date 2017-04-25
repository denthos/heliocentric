#include "game_channels.h"
#include "game_server.h"
#include <chrono>
#include <thread>

void run_server(GameServer* server) {
	server->run();
}

void stop_server(GameServer* server) {
	std::this_thread::sleep_for(std::chrono::seconds(5));
	server->end_game();
}

int main() {
	initializeChannels();
	GameServer* server = new GameServer(30, "9876", 5, 10);

	// Creates two threads to show how to stop the server with another thread calling GameServer::end_game()
	std::thread t1(run_server, server);
	std::thread t2(stop_server, server);

	t1.join();
	t2.join();
}