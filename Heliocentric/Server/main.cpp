#include "game_channels.h"
#include "game_server.h"
#include "logging.h"


int main() {
	initializeChannels();

	GameServer server(30, "9876", 5, 10);
	Lib::LOG_DEBUG("Starting server on port ", 9876);


	server.run();
}
