#pragma once

#include <queue>
#include <memory>
#include <functional>
#include <atomic>
#include <ctime>
#include <initializer_list>

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
#include "unit_manager.h"

#include "debug_pause.h"
#include "player_command.h"
#include "unit_command.h"

#include "trade_deal.h"

#include "player_client_to_server_xfer.h"

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
	UnitManager unit_manager;
	std::unordered_map<UID, std::unique_ptr<Player>> players;
	std::unordered_map<UID, std::unique_ptr<GameObject>> game_objects;

	Lib::LockedItem<std::queue<std::shared_ptr<GameObjectUpdate>>> object_updates;

	struct UpdateToSend {
		std::function<void(SunNet::ChanneledSocketConnection_p)> send_function;
		std::vector<SunNet::ChanneledSocketConnection_p> intended_recipients;
	};

	Lib::LockedItem<std::queue<UpdateToSend>> updates_to_send;

	template <typename TUpdate>
	void sendUpdateToConnection(std::shared_ptr<TUpdate> update, SunNet::ChanneledSocketConnection_p connection) {
		try {
			connection->channeled_send<TUpdate>(update.get());
		}
		catch (const std::exception& e) {
			/* 
			If there is an error sending data to the client, let's log it and forget about it.
			Maybe the error is only transient :)

			RIP game experience
			*/
			LOG_ERR("Error when trying to send an update to a client: ", e.what());
		}
	}

	template <typename TUpdate>
	void makeUpdateToSend(std::shared_ptr<TUpdate> update, std::initializer_list<SunNet::ChanneledSocketConnection_p> intended_recipients) {
		UpdateToSend sendable_update{
			std::bind(&GameServer::sendUpdateToConnection<TUpdate>, this, update, std::placeholders::_1),
			intended_recipients
		};

		this->updates_to_send.get_item().push(sendable_update);
	}

	void handleGamePause(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<DebugPause> pause);
	void handlePlayerCommand(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<PlayerCommand> creation_command);
	void handleUnitCommand(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<UnitCommand> creation_command);
	void handleTradeDeal(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<TradeDeal> trade_deal);

	template <typename TUpdate>
	void addUpdateToSendQueue(std::shared_ptr<TUpdate> update, std::initializer_list<SunNet::ChanneledSocketConnection_p> intended_recipients = {}) {
		std::lock_guard<std::mutex> guard(this->updates_to_send.get_lock());
		this->makeUpdateToSend(update, intended_recipients);
	}

	template <typename Iter>
	void addUpdateToSendQueue(Iter& begin, Iter& end, std::initializer_list<SunNet::ChanneledSocketConnection_p> intended_recipients = {}) {
		std::lock_guard<std::mutex> guard(this->updates_to_send.get_lock());
		for (auto& it = begin; it != end; ++it) {
			this->makeUpdateToSend(*it, intended_recipients);
		}
	}

	void subscribeToChannels();
	void performUpdates();
	void sendUpdates();

	std::atomic<bool> game_paused;
	std::atomic<bool> game_running;
	int tick_duration;

	Player* extractPlayerFromConnection(SunNet::ChanneledSocketConnection_p, bool retry=false);
	void handleReceivePlayerClientToServerTransfer(SunNet::ChanneledSocketConnection_p, std::shared_ptr<PlayerClientToServerTransfer>);

protected:
	/**** Handlers for ChanneledServer ****/
	virtual void handleClientDisconnect(SunNet::ChanneledSocketConnection_p client);

	/**** Handlers for server class ****/
	virtual void handle_server_connection_error() {
		LOG_ERR("Server error");
	}

	virtual void handle_server_disconnect() {
		LOG_ERR("Server disconnect...");
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
