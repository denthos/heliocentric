#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "unit_creation_update.h"
#include "unit_update.h"
#include "game_server.h"

#include "player_id_confirmation.h"

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
	std::lock_guard<std::mutex> guard(connections.get_lock());
	for (auto& connection : connections.get_item().first) {
		connection.second.reset();
	}
}

void GameServer::handleClientDisconnect(SunNet::ChanneledSocketConnection_p client) {
	/* If a client wishes to disconnect, we just drop them entirely. */
	LOG_DEBUG("Disconnecting client");
	std::lock_guard<std::mutex> guard(connections.get_lock());
	auto id_it = connections.get_item().second.find(client);
	if (id_it != connections.get_item().second.end()) {
		players.erase(id_it->second);
		connections.get_item().first.erase(id_it->second);
		connections.get_item().second.erase(id_it);
	}
}

void GameServer::handle_channeledclient_error(SunNet::ChanneledSocketConnection_p client) {
	/* If a client error occurs, we just drop them entirely. */
	LOG_ERR("Client error!");
	this->handleClientDisconnect(client);
}

void GameServer::handle_channeledclient_connect(SunNet::ChanneledSocketConnection_p client) {
	/* A new client is connected! Assign them an ID */
	LOG_DEBUG("A new client connected.");
	std::unique_ptr<Player> player = std::make_unique<Player>("PLAYER_NAME");

	{
		std::lock_guard<std::mutex> guard(connections.get_lock());
		connections.get_item().first[player->getID()] = client;
		connections.get_item().second[client] = player->getID();
	}

	/* 
	Now that we've saved all the player's information, let's send the player to the client
	so that the client knows what its player ID is. In return, the client will send us information
	about the player (like his name)
	*/
	std::shared_ptr<PlayerIDConfirmation> id_confirm = std::make_shared<PlayerIDConfirmation>();
	id_confirm->id = player->getID();

	LOG_DEBUG("Assigned id ", player->getID(), " to new player.. sending confirmation");
	this->addUpdateToSendQueue(id_confirm, { client });

	players[player->getID()] = std::move(player);
}

Player* GameServer::extractPlayerFromConnection(SunNet::ChanneledSocketConnection_p connection, bool retrying) {
	UID player_id;
	{
		/* First, let's extract the player id from the connection */
		std::lock_guard<std::mutex> guard(this->connections.get_lock());
		auto& player_id_it = this->connections.get_item().second.find(connection);

		if (player_id_it == this->connections.get_item().second.end()) {
			if (retrying) {
				/*
				Okay. We've had this problem once and we tried to reconnect the user. Something is seriously
				wrong that the devs need to fix. Let's log this and return something bad. This should _never_
				happen and we should be unforgiving about crashing.
				*/
				LOG_ERR("Could not find player for connection, even after retry.");
				return NULL;
			}

			/*
			The player was not found. As a rudimentary solution, let's log the problem and
			reconnect the connection, considering them a "new" player. We will then retry this
			operation
			*/
			LOG_WARN("Could not find player for connection. Gracefully retrying...");
			this->handle_client_connect(connection);
			return this->extractPlayerFromConnection(connection, true);
		}

		player_id = player_id_it->second;
	}

	/* We now have the player id. Let's look it up. */
	auto& player_it = this->players.find(player_id);

	if (player_it == this->players.end()) {
		/*
		We've had this error before. Something is very wrong that the devs need to fix. Let's log it
		and be unforgiving about returning garbage.
		*/
		if (retrying) {
			LOG_ERR("Could not find player with UID ", player_id, " even after retry.");
			return NULL;
		}
		/* 
		WOAH THERE. We have a Connection->UID mapping for the player, but we don't know
		about the UID. This is weird, but we are going to reconnect the user.
		*/
		this->handleClientDisconnect(connection);
		this->handle_client_connect(connection);
		return this->extractPlayerFromConnection(connection, true);
	}

	LOG_DEBUG("Receivied valid communcation from player ", player_it->first);
	return player_it->second.get();
}

void GameServer::handleReceivePlayerClientToServerTransfer(
	SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<PlayerClientToServerTransfer> info) {

	Player* player = this->extractPlayerFromConnection(sender);
	LOG_DEBUG("Received player client->server info transfer. Player with ID ", player->getID(), " has name ", info->name);
	info->apply(player);

	/* OK. We've applied the information to the player. Now we'd like to send this information to all other players */
	std::shared_ptr<PlayerUpdate> player_update = std::make_shared<PlayerUpdate>(player->getID(), info->name);

	LOG_DEBUG("Sending information about new player to all others..");
	this->addUpdateToSendQueue(player_update);

	/* OK. Finally, let's send information about _all_ other players to this player */
	LOG_DEBUG("Sending information about all others to new player...");
	for (auto& other_player_it : this->players) {
		if (other_player_it.first == player->getID()) {
			continue;
		}

		std::shared_ptr<PlayerUpdate> player_update = std::make_shared<PlayerUpdate>(other_player_it.first, other_player_it.second->get_name().c_str());
		this->addUpdateToSendQueue(player_update, { sender });
	}
}

void GameServer::performUpdates() {
	if (this->game_paused) {
		return;
	}

	/* First, update the universe */
	this->universe.doLogic();
	this->addUpdateToSendQueue(universe.get_updates().begin(), universe.get_updates().end());

	/* update the unit manager */
	this->unit_manager.doLogic();
	this->addUpdateToSendQueue(unit_manager.get_updates().begin(), unit_manager.get_updates().end());
}

void GameServer::sendUpdates() {
	std::lock_guard<std::mutex> guard(this->updates_to_send.get_lock());
	auto& update_queue = this->updates_to_send.get_item();
	while (!update_queue.empty()) {
		auto& update = update_queue.front();
		{
			/* 
			If the update is intended to be sent to a particular person, send it to them.
			Otherwise, send it to all connections
			*/
			if (update.intended_recipients.size() > 0) {
				for (auto& recipient : update.intended_recipients) {
					update.send_function(recipient);
				}
			}
			else {
				std::lock_guard<std::mutex> guard(connections.get_lock());
				for (auto& player_conn : connections.get_item().first) {
					update.send_function(player_conn.second);
				}
			}
		}

		update_queue.pop();
	}
}


void GameServer::run() {
	this->open();
	this->serve();

	std::clock_t tick_start_time;
	std::clock_t tick_elapsed_time;

	while (game_running) {
		tick_start_time = std::clock();

		// Add server logic
		performUpdates();
		sendUpdates();

		tick_elapsed_time = std::clock() - tick_start_time;
		if (tick_elapsed_time > tick_duration) {
			LOG_WARN("Tick processing took ", tick_elapsed_time - tick_duration, "ms longer than expected");
		}

		while ((tick_elapsed_time = std::clock() - tick_start_time) < tick_duration);
	}
	LOG_DEBUG("Server exited safely");
}

void GameServer::end_game() {
	game_running = false;
	this->close();
}

void GameServer::handleGamePause(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<DebugPause> pause) {
	{
		std::lock_guard<std::mutex>(this->connections.get_lock());
		LOG_DEBUG("Game paused by connection ", this->connections.get_item().second[sender]);
	}
	this->game_paused = !this->game_paused;
}

void GameServer::handlePlayerCommand(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<PlayerCommand> command) {
	LOG_DEBUG("Received a player command.");

	/* This currently only handles create command */
	switch (command->command_type) {
		case PlayerCommand::CMD_CREATE: {
			LOG_DEBUG("Player command type: CMD_CREATE.");
			/* We need to use the creation_command's ID to create a unit. For now, let's just create a unit */
			Player* owner = this->extractPlayerFromConnection(sender);

			std::shared_ptr<UnitCreationUpdate>update = unit_manager.add_unit(command, owner);
			this->addUpdateToSendQueue(update);
			break;
		}
		case PlayerCommand::CMD_TRADE: {
			LOG_DEBUG("Player command type: CMD_TRADE");

			/* For now, let's just assume that the recipient is gonna accept this trade deal. */
			UID sender_id;
			SunNet::ChanneledSocketConnection_p recipient;
			{
				/* These operations need to be locked */
				std::lock_guard<std::mutex> guard(this->connections.get_lock());
				sender_id = (connections.get_item().second)[sender]; // Get UID of the sender

				if ((connections.get_item().first).find(command->trade_recipient) != (connections.get_item().first).end()) {
					recipient = (connections.get_item().first)[command->trade_recipient];
				}
				else {
					/* UID does not correspond to a player */
					LOG_ERR("Invalid player UID.");
				}
			}

			std::shared_ptr<PlayerUpdate> seller_update = std::make_shared<PlayerUpdate>(sender_id,
				command->trade_selling, -command->trade_sell_amount);
			std::shared_ptr<PlayerUpdate> recipient_update = std::make_shared<PlayerUpdate>(command->trade_recipient,
				command->trade_selling, command->trade_sell_amount);

			this->addUpdateToSendQueue(seller_update, { sender });
			this->addUpdateToSendQueue(recipient_update, { recipient });
			break;
		}
		default:
			LOG_ERR("Invalid player command.");
		}
}
  
void GameServer::handleUnitCommand(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<UnitCommand> command) {
	LOG_DEBUG("Received a unit command.");

	/* This currently only handles attack and move commands */
	switch (command->command_type) {
		case UnitCommand::CMD_ATTACK:
			LOG_DEBUG("Unit command type: CMD_ATTACK");
			break;
		case UnitCommand::CMD_MOVE:
			LOG_DEBUG("Unit command type: CMD_MOVE");
			// TODO: Delegate to UnitManager
			unit_manager.do_move(command.get()->initiator, command.get()->destination_x, command.get()->destination_y, command.get()->destination_z);
			break;
		default:
			LOG_ERR("Invalid unit command.");
	}
}

void GameServer::handleTradeDeal(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<TradeDeal> trade_deal) {
	LOG_DEBUG("Received a trade deal.");

	if (trade_deal->isAccepted()) {
		/* Trade deal accepted */
		if (players.find(trade_deal->recipient) == players.end()) {
			/* If UID does not correspond to a player */
			LOG_ERR("Invalid player UID.");
		}
	}
	else {
		/* Trade deal declined */

	}
}

void GameServer::subscribeToChannels() {
	/* Subscribe to important channels */
	this->subscribe<DebugPause>(std::bind(&GameServer::handleGamePause, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<PlayerCommand>(std::bind(&GameServer::handlePlayerCommand, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<UnitCommand>(std::bind(&GameServer::handleUnitCommand, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<PlayerClientToServerTransfer>(std::bind(&GameServer::handleReceivePlayerClientToServerTransfer, this, std::placeholders::_1, std::placeholders::_2));
}
