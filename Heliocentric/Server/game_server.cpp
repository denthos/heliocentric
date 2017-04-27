#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "game_server.h"

GameServer::GameServer(int tick_duration, std::string port, int listen_queue, int poll_timeout) :
	SunNet::ChanneledServer<SunNet::TCPSocketConnection>("0.0.0.0", port, listen_queue, poll_timeout), game_paused(false) {

	game_running = true;
	this->tick_duration = tick_duration;
	this->subscribeToChannels();
}

GameServer::~GameServer() {
	/* 
	Let's be a good citizen. When we are closing, let's _gracefully_ close
	all the player connections ^^
	*/
	std::lock_guard<std::mutex>(connections.get_lock());
	for (auto& connection : connections.get_item().first) {
		connection.second.reset();
	}
}

void GameServer::handleClientDisconnect(SunNet::ChanneledSocketConnection_p client) {
	/* If a client wishes to disconnect, we just drop them entirely. */
	Lib::LOG_DEBUG("Disconnecting client");
	std::lock_guard<std::mutex>(connections.get_lock());
	auto id_it = connections.get_item().second.find(client);
	if (id_it != connections.get_item().second.end()) {
		connections.get_item().first.erase(id_it->second);
		connections.get_item().second.erase(id_it);
	}
}

void GameServer::handle_channeledclient_error(SunNet::ChanneledSocketConnection_p client) {
	/* If a client error occurs, we just drop them entirely. */
	Lib::LOG_ERR("Client error!");
	this->handleClientDisconnect(client);
}

void GameServer::handle_channeledclient_connect(SunNet::ChanneledSocketConnection_p client) {
	/* A new client is connected! Assign them an ID */
	Lib::LOG_DEBUG("A new client connected.");
	std::unique_ptr<Player> player = std::make_unique<Player>("PLAYER_NAME");

	{
		std::lock_guard<std::mutex>(connections.get_lock());
		connections.get_item().first[player->getID()] = client;
		connections.get_item().second[client] = player->getID();
	}

	players[player->getID()] = std::move(player);
}

void GameServer::performUpdates() {
	if (this->game_paused) {
		return;
	}

	/* First, update the universe */
	this->universe.doLogic();
	for (auto& universe_update : universe.get_updates()) {
		this->updates_to_send.push(this->makeUpdateSendFunction(universe_update));
	}
}

void GameServer::sendUpdates() {
	while (!updates_to_send.empty()) {
		auto& update = updates_to_send.front();
		{
			std::lock_guard<std::mutex>(connections.get_lock());
			for (auto& player_conn : connections.get_item().first) {
				update(player_conn.second);
			}
		}

		updates_to_send.pop();
	}
}


void GameServer::run() {
	this->open();
	this->serve(10);

	std::clock_t tick_start_time;
	std::clock_t tick_elapsed_time;

	while (game_running) {
		tick_start_time = std::clock();

		// Add server logic
		performUpdates();
		sendUpdates();

		while ((tick_elapsed_time = std::clock() - tick_start_time) < tick_duration);
		if (tick_elapsed_time > tick_duration)
			Lib::LOG_WARN("Tick processing took ", tick_elapsed_time - tick_duration, "ms longer than expected");
	}
	Lib::LOG_DEBUG("Server exited safely");
}

void GameServer::end_game() {
	game_running = false;
	this->close();
}

void GameServer::handleGamePause(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<DebugPause> pause) {
	{
		std::lock_guard<std::mutex>(this->connections.get_lock());
		Lib::LOG_DEBUG("Game paused by connection ", this->connections.get_item().second[sender]);
	}
	this->game_paused = !this->game_paused;
}

void GameServer::subscribeToChannels() {
	this->subscribe<DebugPause>(std::bind(&GameServer::handleGamePause, this, std::placeholders::_1, std::placeholders::_2));
}