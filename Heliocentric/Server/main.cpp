#include "game_channels.h"
#include "game_server.h"

#include "logging.h"
#include "ini_parser.h"


int main() {
	initializeChannels();

	std::string port = Lib::INIParser::getInstance().get<std::string>("ServerPort");
	GameServer server(30, port, 5, 10);
	Lib::LOG_DEBUG("Starting server on port ", 9876);


	server.run();
}
