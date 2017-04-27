#pragma once

#include <queue>
#include <memory>
#include <functional>
#include <atomic>
#include <ctime>

#include "logging.h"
#include "player.h"
#include "identifiable.h"
#include "channeled_server.h"
#include "tcp_socket_connection.hpp"
#include "locked_item.h"
#include "player_update.h"
#include "planet_update.h"
#include "game_object_update.h"
#include "universe.h"

#include "debug_pause.h"

class GameServer : public SunNet::ChanneledServer<SunNet::TCPSocketConnection> {
	/*
	The server will run in a loop with a loop timer (30ms). During each loop we will do the following:
	1. Process all requests (update object database, etc.)
	2. Send updates to connected clients
	3. Sleep until 30ms requirement is met
	During this time, all client messages will be placed in a queue (the queue will be locked during
	step 1). The queue will be emptied in step 1. Thus, we can receive client requests while sending out
	updates and sleeping.
	*/


private:

	Lib::LockedItem<std::pair<
		std::unordered_map<UID, SunNet::ChanneledSocketConnection_p>,
		std::unordered_map<SunNet::ChanneledSocketConnection_p, UID>>> connections;

	Universe universe;
	std::unordered_map<UID, std::unique_ptr<Player>> players;
	std::unordered_map<UID, std::unique_ptr<GameObject>> game_objects;

	Lib::LockedItem<std::queue<std::shared_ptr<GameObjectUpdate>>> object_updates;
	std::queue<std::function<void(SunNet::ChanneledSocketConnection_p)>> updates_to_send;

	template <typename TUpdate>
	void sendUpdateToConnection(std::shared_ptr<TUpdate> update, SunNet::ChanneledSocketConnection_p connection) {
		try {
			connection->channeled_send<TUpdate>(update.get());
		}
		catch (const std::exception& e) {
			/* 
			If there is an error sending data to the client, let's log it and forget about it.
			Maybe the error is only transient :)
			*/
			Lib::LOG_ERR("Error when trying to send an update to a client", e.what());
		}
	}

	template <typename TUpdate>
	std::function<void(SunNet::ChanneledSocketConnection_p)> makeUpdateSendFunction(std::shared_ptr<TUpdate> update) {
		return std::bind(&GameServer::sendUpdateToConnection<TUpdate>, this, update, std::placeholders::_1);
	}

	void handleGamePause(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<DebugPause> pause);

	void subscribeToChannels();
	void performUpdates();
	void sendUpdates();

	std::atomic<bool> game_paused;
	std::atomic<bool> game_running;
	int tick_duration;

protected:
	/**** Handlers for ChanneledServer ****/
	virtual void handleClientDisconnect(SunNet::ChanneledSocketConnection_p client);

	/**** Handlers for server class ****/
	virtual void handle_server_connection_error() {
		Lib::LOG_ERR("Server error");
	}

	virtual void handle_channeledclient_error(SunNet::ChanneledSocketConnection_p client);
	virtual void handle_channeledclient_connect(SunNet::ChanneledSocketConnection_p client);
	virtual void handle_poll_timeout() {}

public:
	GameServer(int tick_duration, std::string port, int listen_queue, int poll_timeout);
	~GameServer();

	void run();
	void end_game();
};
