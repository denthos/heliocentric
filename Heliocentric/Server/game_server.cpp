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

void GameServer::handleReceivePlayerClientToServerTransfer(
	SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<PlayerClientToServerTransfer> info) {

	std::lock_guard<std::mutex> guard(this->connections.get_lock());
	auto& player_id_it = this->connections.get_item().second.find(sender);
	if (player_id_it == this->connections.get_item().second.end()) {
		/* What the.. we received player info from a connection we don't know about? This is weird. */
		LOG_WARN("Received player info from connection we haven't heard of before...");

		/* 
		Luckily, if we send them another ID confirmation, the client will take the new ID and overwrite
		what they already have. Let's just act like they're brand new.
		*/
		this->handle_channeledclient_connect(sender);
		return;
	}

	auto& player_it = this->players.find(player_id_it->second);
	if (player_it == this->players.end()) {
		/* This is even weirder.. we received player info from a connection we know about, but we don't know about the player... */
		LOG_WARN("Received player info for unknown player ID: ", player_id_it->second);

		/*
		In this situation, it seems like the client simply ACKd a different ID number than we gave them.
		Let's start the process over for them. We will remove their data and act like they're a new connection.
		*/
		this->handleClientDisconnect(sender);
		this->handle_channeledclient_connect(sender);
		return;
	}

	LOG_DEBUG("Received player client->server info transfer. Player with ID ", player_it->first, " has name ", info->name);
	info->apply(player_it->second.get());

	/* OK. We've applied the information to the player. Now we'd like to send this information to all other players */
	std::shared_ptr<PlayerUpdate> player_update = std::make_shared<PlayerUpdate>(player_it->first, info->name);

	LOG_DEBUG("Sending information about new player to all others..");
	this->addUpdateToSendQueue(player_update);

	/* OK. Finally, let's send information about _all_ other players to this player */
	LOG_DEBUG("Sending information about all others to new player...");
	for (auto& other_player_it : this->players) {
		if (other_player_it.first == player_it->first) {
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
			LOG_DEBUG("Play command type: CMD_CREATE.");
			/* We need to use the creation_command's ID to create a unit. For now, let's just create a unit */
			// TODO: Create the new unit (should probably be a unique_ptr) and move it into the UnitManager
			// TODO: Queue up UnitUpdates for the unit
			UID unit_owner_id; // ID of the player who sent the command
			{
				/* These operations need to be locked */
				std::lock_guard<std::mutex>(this->connections.get_lock());
				/* Get player ID if player does exist */
				if ((connections.get_item().second).find(sender) != (connections.get_item().second).end()) {
					unit_owner_id = (connections.get_item().second)[sender];
				}
				else {
					LOG_ERR("Player does not exist");
					return;
				}
			}

			/* Let's just create a variable for the unit position, because it's so long. */
			glm::vec3 unit_position(command->create_location_x, command->create_location_y, command->create_location_z);
			std::unique_ptr<Unit> new_unit = std::make_unique<Unit>(unit_position, players[unit_owner_id].get(),
				config_core.get<int>("Unit_attack"), config_core.get<int>("Unit_defense"),
				config_core.get<int>("Unit_range"), config_core.get<int>("Unit_health"),
				config_core.get<int>("Unit_movement_speed")); // Creates a new unit
			// TODO: Put this unit into unit manager

			std::shared_ptr<UnitCreationUpdate> update = std::make_shared<UnitCreationUpdate>(new_unit->getID(),
				command->create_location_x, command->create_location_y, command->create_location_z, unit_owner_id,
				new_unit->get_combat_attack(), new_unit->get_combat_defense(), new_unit->get_combat_range(),
				new_unit->get_health(), new_unit->get_movement_speed_max()); // Creates an update for the unit to be sent to client

			this->addUpdateToSendQueue(update, { sender });
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
			break;
		default:
			LOG_ERR("Invalid unit command.");
	}
}

void GameServer::subscribeToChannels() {
	/* Subscribe to important channels */
	this->subscribe<DebugPause>(std::bind(&GameServer::handleGamePause, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<PlayerCommand>(std::bind(&GameServer::handlePlayerCommand, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<UnitCommand>(std::bind(&GameServer::handleUnitCommand, this, std::placeholders::_1, std::placeholders::_2));
}
