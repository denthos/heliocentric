#pragma once

#include <queue>
#include <memory>
#include <functional>
#include <atomic>
#include <ctime>
#include <initializer_list>

#include "game_session.h"
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
#include "city_manager.h"
#include "slot_update.h"
#include "player_color.h"

#include "debug_pause.h"
#include "player_command.h"
#include "unit_command.h"
#include "trade_command.h"
#include "settle_city_command.h"

#include "trade_data.h"

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
	Lib::Lock<std::pair<
		std::unordered_map<UID, SunNet::ChanneledSocketConnection_p>,
		std::unordered_map<SunNet::ChanneledSocketConnection_p, UID>>> connections;

	Universe universe;

	UnitManager unit_manager;
	CityManager city_manager;

	AttackableGameObject* get_attackable(UID uid) const;
	PlayerColor::Color nextPlayerColor = PlayerColor::FIRST;

	bool updatePlayerResources(std::vector<std::shared_ptr<PlayerUpdate>>& player_updates, std::vector<std::shared_ptr<SlotUpdate>>& slot_updates);
	int lastResourceUpdateTick = 0;

	std::unordered_map<UID, std::unique_ptr<Player>> players;
	std::unordered_map<UID, Slot*> slots;
	std::unordered_map<UID, std::unique_ptr<GameObject>> game_objects;

	struct UpdateToSend {
		std::function<void(SunNet::ChanneledSocketConnection_p)> send_function;
		std::vector<SunNet::ChanneledSocketConnection_p> intended_recipients;
	};


	Lib::Lock<std::queue<UpdateToSend>> updates_to_send;
	Lib::Lock<std::queue<std::function<void()>>> update_process_queue;
	void addFunctionToProcessQueue(std::function<void()> work);

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

	template <typename TUpdate, typename TQueue>
	void makeUpdateAndInsertIntoQueue(std::shared_ptr<TUpdate> update, std::initializer_list<SunNet::ChanneledSocketConnection_p> intended_recipients, TQueue& queue) {
		UpdateToSend sendable_update{
			std::bind(&GameServer::sendUpdateToConnection<TUpdate>, this, update, std::placeholders::_1),
			intended_recipients
		};

		queue.push(sendable_update);
	}

	void handleGamePause(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<DebugPause> pause);
	void handlePlayerCommand(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<PlayerCommand> command);
	void handleUnitCommand(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<UnitCommand> command);
	void handleTradeCommand(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<TradeCommand> command);
	void handleTradeData(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<TradeData> trade_deal);

	template <typename TUpdate>
	void addUpdateToSendQueue(std::shared_ptr<TUpdate> update, std::initializer_list<SunNet::ChanneledSocketConnection_p> intended_recipients = {}) {
		auto updateQueue = Lib::key_acquire(this->updates_to_send);
		this->makeUpdateAndInsertIntoQueue(update, intended_recipients, updateQueue.get());
	}

	template <typename Iter>
	void addUpdateToSendQueue(Iter& begin, Iter& end, std::initializer_list<SunNet::ChanneledSocketConnection_p> intended_recipients = {}) {
		auto updateQueue = Lib::key_acquire(this->updates_to_send);
		for (auto& it = begin; it != end; ++it) {
			this->makeUpdateAndInsertIntoQueue(*it, intended_recipients, updateQueue.get());
		}
	}

	void subscribeToChannels();
	void performUpdates();
	void sendUpdates();

	/* Checks victory condition every server tick */
	void checkVictory();

	GameSession* game; // Keeps track of all information about current game settings. Deconstructed after game ends.
	std::atomic<bool> server_paused;
	std::atomic<bool> server_running;
	int tick_duration; // Duration of each tick in ms.
	int resource_update_interval_seconds;

	Player* extractPlayerFromConnection(SunNet::ChanneledSocketConnection_p, bool retry=false);
	void handleReceivePlayerClientToServerTransfer(SunNet::ChanneledSocketConnection_p, std::shared_ptr<PlayerClientToServerTransfer>);
	void handleSettleCityCommand(SunNet::ChanneledSocketConnection_p, std::shared_ptr<SettleCityCommand>);

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
	GameServer(int tick_duration, std::string port, int listen_queue, int poll_timeout, int resource_update_interval_seconds);
	~GameServer();

	void run();
	void shut_down();
};
