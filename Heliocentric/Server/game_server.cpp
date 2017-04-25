#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "game_server.h"

GameServer::GameServer(int tick_duration, std::string port, int listen_queue, int poll_timeout) : 
	SunNet::ChanneledServer<SunNet::TCPSocketConnection>("0.0.0.0", port, listen_queue, poll_timeout) {

	game_running = true;
	this->tick_duration = tick_duration;

	/* Create the earth */
	std::unordered_map<UID, Slot*> slots;

	std::unique_ptr<Planet> earth = std::make_unique<Planet>(glm::vec3(50.0f, 0.0f, 0.0f), "earth", slots);
	game_objects[earth->getID()] = std::move(earth);

	std::unique_ptr<Planet> mars = std::make_unique<Planet>(glm::vec3(100.0f, 0.0f, 0.0f), "mars", slots);
	game_objects[mars->getID()] = std::move(mars);
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

void GameServer::movePlanets() {
	int neg = 1;
	for (auto& obj : game_objects) {
		neg *= -1;
		Planet* planet = static_cast<Planet*>(obj.second.get());
		if (planet) {
			/* Orbit the planet around the sun (0,0,0) */
			glm::vec3 currentPosition = planet->get_position();
			glm::vec3 newPosition = (
				glm::rotate(glm::mat4(1.0f),((neg * 2.0f) / 180.0f) * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f)) *
				glm::vec4(currentPosition, 1.0f)
			);

			planet->update_position(newPosition);

			std::shared_ptr<PlanetUpdate> update = std::make_shared<PlanetUpdate>();
			update->id = planet->getID();
			update->x = newPosition.x;
			update->y = newPosition.y;
			update->z = newPosition.z;

			updates_to_send.push(std::bind(&GameServer::sendUpdateToConnection<PlanetUpdate>, this, update, std::placeholders::_1));
		}
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
		movePlanets();
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
