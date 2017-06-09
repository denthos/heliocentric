#include <functional>

#include "unit_creation_update.h"
#include "unit_update.h"
#include "game_server.h"
#include "city.h"
#include "player_id_confirmation.h"
#include "city_creation_update.h"
#include "instant_laser_attack.h"
#include "new_player_info_update.h"
#include "unit_spawner_update.h"

#include "game_over_update.h"

GameServer::GameServer(int tick_duration, std::string port, int listen_queue, int poll_timeout, int resource_update_interval) :
	SunNet::ChanneledServer<SunNet::TCPSocketConnection>("0.0.0.0", port, listen_queue, poll_timeout), server_paused(false), resource_update_interval_seconds(resource_update_interval) {

	this->game = new GameSession();

	for (auto& planet_iter : this->universe.get_planets()) {
		for (auto& slot_iter : planet_iter->get_slots_const()) {
			this->slots.insert(std::make_pair(slot_iter.first, slot_iter.second));
		}
	}

	server_running = true;
	this->tick_duration = tick_duration;
	this->subscribeToChannels();

	this->unit_manager = std::make_unique<UnitManager>();
	this->city_manager = std::make_unique<CityManager>(unit_manager.get());
	this->player_manager = std::make_unique<PlayerManager>();
}

GameServer::~GameServer() {
	/* 
	Let's be a good citizen. When we are closing, let's _gracefully_ close
	all the player connections ^^
	*/
	for (auto& connection : connections.first) {
		connection.second.reset();
	}
}

std::shared_ptr<AttackableGameObject> GameServer::get_attackable(UID uid) const {
	std::shared_ptr<AttackableGameObject> ret = unit_manager->get_unit(uid);
	return (ret == nullptr) ? city_manager->get_city(uid) : ret;
}


void GameServer::handleClientDisconnect(SunNet::ChanneledSocketConnection_p client) {
	/* If a client wishes to disconnect, we just drop them entirely. */
	LOG_DEBUG("Disconnecting client");
	auto id_it = connections.second.find(client);
	if (id_it != connections.second.end()) {
		//TODO: Properly erase the player. For now, we won't erase him since it breaks things
		//players.erase(id_it->second);
		connections.first.erase(id_it->second);
		connections.second.erase(id_it);
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
	std::shared_ptr<Player> player = std::make_shared<Player>(player_manager.get(), "PLAYER_NAME", this->nextPlayerColor);
	/* Register this player to player manager. */
	player_manager->register_player(player);

	int nextColor = static_cast<int>(this->nextPlayerColor) + 1;
	if (nextColor >= PlayerColor::NUM_COLORS) {
		LOG_ERR("NO MORE COLORS AVAILABLE!");

		nextColor = static_cast<int>(PlayerColor::FIRST);

	}

	this->nextPlayerColor = static_cast<PlayerColor::Color>(nextColor);

	{
		connections.first[player->getID()] = client;
		connections.second[client] = player->getID();
	}

	/* 
	Now that we've saved all the player's information, let's send the player to the client
	so that the client knows what its player ID is. In return, the client will send us information
	about the player (like his name)
	*/
	std::shared_ptr<PlayerIDConfirmation> id_confirm = std::make_shared<PlayerIDConfirmation>();
	id_confirm->id = player->getID();
	id_confirm->color = player->getColor();

	LOG_DEBUG("Assigned id ", player->getID(), " to new player.. sending confirmation");
	this->addUpdateToSendQueue(id_confirm, { client });

	players[player->getID()] = player;
}

Player* GameServer::extractPlayerFromConnection(SunNet::ChanneledSocketConnection_p sender, bool retrying) {
	UID player_id;
	{
		/* First, let's extract the player id from the connection */
		auto& player_id_it = connections.second.find(sender);

		if (player_id_it == connections.second.end()) {
			if (retrying) {
				/*
				Okay. We've had this problem once and we tried to reconnect the user. Something is seriously
				wrong that the devs need to fix. Let's log this and return something bad. This should _never_
				happen and we should be unforgiving about crashing.
				*/
				LOG_ERR("Could not find player for connection, even after retry.");
				return NULL;
			}

			/* What the.. we received player info from a connection we don't know about? This is weird. */
			LOG_WARN("Received player info from connection we haven't heard of before...");
			/*
			The player was not found. As a rudimentary solution, let's log the problem and
			reconnect the connection, considering them a "new" player. We will then retry this
			operation
			*/
			LOG_WARN("Could not find player for connection. Gracefully retrying...");
			this->handle_client_connect(sender);
			return this->extractPlayerFromConnection(sender, true);
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
		this->handleClientDisconnect(sender);
		this->handle_client_connect(sender);
		return this->extractPlayerFromConnection(sender, true);
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
	std::shared_ptr<NewPlayerInfoUpdate> player_update = std::make_shared<NewPlayerInfoUpdate>(player->getID(), info->name, player->getColor());

	LOG_DEBUG("Sending information about new player to all others..");
	this->addUpdateToSendQueue(player_update);

	/* OK. Finally, let's send information about _all_ other players to this player */
	LOG_DEBUG("Sending information about all others to new player...");
	for (auto& other_player_it : this->players) {
		if (other_player_it.first == player->getID()) {
			continue;
		}

		std::shared_ptr<NewPlayerInfoUpdate> player_update = std::make_shared<NewPlayerInfoUpdate>(other_player_it.first, other_player_it.second->get_name(), other_player_it.second->getColor());
		this->addUpdateToSendQueue(player_update, { sender });
	}
}

void GameServer::performUpdates() {
	if (this->server_paused) {
		return;
	}

	/* First, update the universe */
	this->universe.doLogic();

	/* update player manager */
	this->player_manager->doLogic();

	/* update the city manager */
	this->city_manager->doLogic();

	/* update the unit manager */
	this->unit_manager->doLogic();

	this->addUpdateToSendQueue(universe.get_updates().begin(), universe.get_updates().end());
	this->addUpdateToSendQueue(unit_manager->get_updates().begin(), unit_manager->get_updates().end());

	this->addUpdateToSendQueue(city_manager->get_updates().begin(), city_manager->get_updates().end());
	this->addUpdateToSendQueue(city_manager->getCreationUpdates().begin(), city_manager->getCreationUpdates().end());


	this->addUpdateToSendQueue(player_manager->getPlayerScoreUpdates().begin(), player_manager->getPlayerScoreUpdates().end());

	/* These updates should only be sent to the relevant players */
	for (auto& spawner_update : city_manager->getSpawnerUpdates()) {
		auto city = city_manager->get_city(spawner_update->id);
		if (!city) {
			continue;
		}

		auto player = city->get_player();
		if (!player) {
			continue;
		}

		UID owner_id = player->getID();
		this->addUpdateToSendQueue(spawner_update, { owner_id });
	}

	for (auto& research_update : player_manager->getPlayerResearchUpdates()) {
		UID owner_id = research_update->id;
		this->addUpdateToSendQueue(research_update, { owner_id });
	}

	this->addUpdateToSendQueue(this->game->timeUpdate);

	/* Give players resources based on their owned cities */
	std::vector<std::shared_ptr<PlayerUpdate>> player_updates;
	std::vector<std::shared_ptr<SlotUpdate>> slot_updates;
	if (updatePlayerResources(player_updates, slot_updates)) {
		this->addUpdateToSendQueue(player_updates.begin(), player_updates.end());
		this->addUpdateToSendQueue(slot_updates.begin(), slot_updates.end());
	}

}

bool GameServer::updatePlayerResources(std::vector<std::shared_ptr<PlayerUpdate>>& player_updates, std::vector<std::shared_ptr<SlotUpdate>>& slot_updates) {
	int update_interval_ticks = (this->resource_update_interval_seconds * 1000) / tick_duration;

	if (++lastResourceUpdateTick < update_interval_ticks) {
		return false;
	}

	for (auto& city_pair : city_manager->get_cities()) {

		if (!city_pair.second) {
			LOG_ERR("Failed to update resources for city with UID <", city_pair.first, ">");
			continue;
		}

		city_pair.second->extractResourcesFromSlotAndCreateUpdates(player_updates, slot_updates);
	}

	lastResourceUpdateTick = 0;
	return true;
}

void GameServer::sendUpdates() {
	while (!updates_to_send.empty()) {
		auto& update = updates_to_send.front();
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
				for (auto& player_conn : connections.first) {
					update.send_function(player_conn.second);
				}
			}
		}

		updates_to_send.pop();
	}
}

void GameServer::endGameIfGameOver() {
	UID winnerUID;
	if (!checkVictory(winnerUID)) {
		return;
	}

	game->game_running = false;

	/* Now we need to give a "GameOver" message to all players */
	std::shared_ptr<GameOverUpdate> update = std::make_shared<GameOverUpdate>();
	update->winnerID = winnerUID;

	this->addUpdateToSendQueue(update);
	sendUpdates();
}

bool GameServer::checkVictory(UID& winner) {
	/* The order of these conditions determines priority of victory conditions. */
	if (game->domination_victory) {
		return false;
	}

	if (game->science_victory) {
		return false;
	}

	if (game->economic_victory) {
		return false;
	}

	if (game->time_victory) {
		if (game->time_limit_reached()) {

			/* Player with the highest player score wins. */
			float highest_score = 0.0f;
			UID winner_candidate = INVALID_ID;
			for (auto& it : players) {
				if (it.second->get_player_score() > highest_score) {
					highest_score = it.second->get_player_score();
					winner_candidate = it.second->getID();
				}
			}

			winner = winner_candidate;
			return true;
		}
		else {
			return false;
		}
	}
	else{
		return false;
	}
}


void GameServer::run() {
	this->open();
	this->serve();

	std::chrono::time_point<std::chrono::system_clock> tick_start_time;

	while (server_running) {
		/* Temporary solution to stop server from terminating after game ends */
		if (!game->game_running) {
			continue;
		}

		tick_start_time = std::chrono::system_clock::now();

		game->increment_time(tick_duration);

		// Add server logic

		bool has_more_updates = true;
		while (has_more_updates) {
			has_more_updates = this->poll();
		}

		performUpdates();
		sendUpdates();
		endGameIfGameOver();

		auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - tick_start_time);

		if (elapsed_time.count() > tick_duration) {
			LOG_WARN("Tick processing took ", elapsed_time.count() - tick_duration, "ms longer than expected");
		}
		
		while ((elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - tick_start_time)).count() < tick_duration);
	}
	LOG_DEBUG("Server exited safely");
}

void GameServer::shut_down() {
	server_running = false;
	this->close();
}

void GameServer::handleGamePause(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<DebugPause> pause) {
	{
		LOG_DEBUG("Game paused by connection ", connections.second[sender]);
	}
	this->server_paused = !this->server_paused;
}

void GameServer::handleSettleCityCommand(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<SettleCityCommand> command) {
	LOG_DEBUG("Received settle city command");

	Player* owning_player = this->extractPlayerFromConnection(sender);

	/* Create the new city */
	auto& slot_iter = this->slots.find(command->initiator);
	if (slot_iter == this->slots.end()) {
		LOG_ERR("Slot not found");
		return;
	}
	else if (slot_iter->second->hasCity()) {
		LOG_ERR("Slot with UID <", slot_iter->first, "> is occupied.");
		return;
	}

	/* Bundle and send the update */
	try {
		std::shared_ptr<CityCreationUpdate> city_creation_update = city_manager->add_city(owning_player, slot_iter->second, command->city_name);
		this->addUpdateToSendQueue(city_creation_update);
	}
	catch (Player::SettlementLimitReachedException e) {
		LOG_ERR("Player ", owning_player->getID(), " has reached settlement limit.");
	}
}

void GameServer::handlePlayerCommand(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<PlayerCommand> command) {
	LOG_DEBUG("Received a player command.");

	/* This currently only handles create command */
	switch (command->command_type) {
		case PlayerCommand::CMD_CREATE_UNIT: {
			LOG_DEBUG("Player command type: CMD_CREATE_UNIT.");
			/* We need to use the creation_command's ID to create a unit. For now, let's just create a unit */
			Player* owner = this->extractPlayerFromConnection(sender);

				//TODO: Make all managers know about other managers (eg playermanager) so that this is cleaner
				UnitType* type = UnitType::getByIdentifier(command->createUnitType);
				if (!owner->can_create_unit(type)) {
					// The player does not have the proper requirements. Bail out!
					return;
				}

				try {
					/* Now we are going to add the unit to the city's queue */
					auto spawnUpdate = this->city_manager->spawnUnit(command);
					this->addUpdateToSendQueue(spawnUpdate);

					/* Now we are going to decrement the player's resources */
					std::vector<std::shared_ptr<PlayerUpdate>> playerResourceUpdates;

					for (auto& resource_pair : type->getBuildRequirements()) {
						owner->change_resource_amount(resource_pair.first, -1 * resource_pair.second);
						playerResourceUpdates.push_back(
							std::make_shared<PlayerUpdate>(owner->getID(), resource_pair.first, owner->get_resource_amount(resource_pair.first))
						);
					}

					this->addUpdateToSendQueue(playerResourceUpdates.begin(), playerResourceUpdates.end());
				}
				catch (City::BadUIDException e) {
					LOG_ERR("Invalid city Id sent to server");
				}
			break;
		}
		case PlayerCommand::CMD_CREATE_BUILDING : {
			LOG_DEBUG("Player command type: CMD_CREATE_BUILDING.");
			Player* owner = this->extractPlayerFromConnection(sender);

				BuildingType* type = BuildingType::getByIdentifier(command->createBuildingType);
				/* Check tech tree */
				if (!type->hasBuildRequirements(owner->getResources())) {
					return;
				}

				try {
					auto spawnUpdate = this->city_manager->spawnBuilding(command);
					this->addUpdateToSendQueue(spawnUpdate);

					std::vector<std::shared_ptr<PlayerUpdate>> playerResourceUpdates;

					for (auto& resource_pair : type->getBuildRequirements()) {
						owner->change_resource_amount(resource_pair.first, -1 * resource_pair.second);
						playerResourceUpdates.push_back(
							std::make_shared<PlayerUpdate>(owner->getID(), resource_pair.first, owner->get_resource_amount(resource_pair.first))
						);
					}

					this->addUpdateToSendQueue(playerResourceUpdates.begin(), playerResourceUpdates.end());
				}
				catch (City::BadUIDException e) {
					LOG_ERR("Invalid city Id sent to server");
				}
			break;
		}
		case PlayerCommand::CMD_TRADE: {
			LOG_DEBUG("Player command type: CMD_TRADE");

			/* For now, let's just assume that the recipient is gonna accept this trade deal. */
			Player* sender_player = this->extractPlayerFromConnection(sender);
			SunNet::ChanneledSocketConnection_p recipient;
			{
				/* These operations need to be locked */

				auto& recipient_iter = connections.first.find(command->trade_recipient);
				if (recipient_iter == connections.first.end()) {
					/* UID does not correspond to a player */
					LOG_ERR("Invalid player UID.");
					return;
				}

				recipient = recipient_iter->second;
			}

			std::shared_ptr<PlayerUpdate> seller_update = std::make_shared<PlayerUpdate>(sender_player->getID(),
				command->trade_selling, -command->trade_sell_amount);
			std::shared_ptr<PlayerUpdate> recipient_update = std::make_shared<PlayerUpdate>(command->trade_recipient,
				command->trade_selling, command->trade_sell_amount);

			this->addUpdateToSendQueue(seller_update);
			this->addUpdateToSendQueue(recipient_update);
			break;
		}
		default:
			LOG_ERR("Invalid player command.");
		}
}
  
void GameServer::handleUnitCommand(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<UnitCommand> command) {
	LOG_DEBUG("Received a unit command.");

	/* This currently only handles attack and move commands */
	std::shared_ptr<AttackableGameObject> target;
	switch (command->command_type) {
		case UnitCommand::CMD_ATTACK:
			LOG_DEBUG("Unit command type: CMD_ATTACK");
			target = get_attackable(command.get()->target);
			if (target)
				unit_manager->do_attack(command.get()->initiator, target);
			break;
		case UnitCommand::CMD_MOVE:
			LOG_DEBUG("Unit command type: CMD_MOVE");
			// TODO: Delegate to UnitManager
			unit_manager->do_move(command.get()->initiator, command.get()->destination_x, command.get()->destination_y, command.get()->destination_z);
			break;
		default:
			LOG_ERR("Invalid unit command.");
	}
}

void GameServer::handleResearchCommand(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<ResearchCommand> command) {
	LOG_DEBUG("Received a research command.");

	Player* player = this->extractPlayerFromConnection(sender);
	player->choose_research(command->tech_id);
}

void GameServer::handleTradeCommand(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<TradeCommand> command) {
	LOG_DEBUG("Received a trade command.");

	/* It's called "recipient" because it's the recipient of this deal, even though its connection is named "sender". */
	Player* recipient = this->extractPlayerFromConnection(sender);
	std::shared_ptr<TradeDeal> trade_deal;
	LOG_DEBUG("Trade deal id is ", command->initiator);
	try {
		trade_deal = recipient->get_trade_deal(command->initiator);
	}
	catch (Identifiable::BadUIDException e) {
		LOG_ERR("Invalid trade deal ID");
	}

	switch (command->command_type) {
		case TradeCommand::CMD_TRADE_ACCEPT: {
			recipient->trade_deal_accept(command->initiator);

			int sender_sell_amount = this->players[trade_deal->get_sender()]->get_resource_amount(trade_deal->get_sell_type());
			int sender_buy_amount = this->players[trade_deal->get_sender()]->get_resource_amount(trade_deal->get_buy_type());
			int receiver_sell_amount = this->players[trade_deal->get_recipient()]->get_resource_amount(trade_deal->get_sell_type());
			int receiver_buy_amount = this->players[trade_deal->get_recipient()]->get_resource_amount(trade_deal->get_buy_type());

			// update server
			this->players[trade_deal->get_sender()]->set_resource_amount(trade_deal->get_sell_type(), sender_sell_amount - trade_deal->get_sell_amount());
			this->players[trade_deal->get_sender()]->set_resource_amount(trade_deal->get_buy_type(), trade_deal->get_buy_amount() + sender_buy_amount);
			this->players[trade_deal->get_recipient()]->set_resource_amount(trade_deal->get_sell_type(), trade_deal->get_sell_amount() + receiver_sell_amount);
			this->players[trade_deal->get_recipient()]->set_resource_amount(trade_deal->get_buy_type(), receiver_buy_amount - trade_deal->get_buy_amount());

			// send update to client
			std::shared_ptr<PlayerUpdate> sender_sell_update = std::make_shared<PlayerUpdate>(trade_deal->get_sender(),
				trade_deal->get_sell_type(), sender_sell_amount - trade_deal->get_sell_amount());
			std::shared_ptr<PlayerUpdate> sender_buy_update = std::make_shared<PlayerUpdate>(trade_deal->get_sender(),
				trade_deal->get_buy_type(), trade_deal->get_buy_amount() + sender_buy_amount);
			std::shared_ptr<PlayerUpdate> recipient_sell_update = std::make_shared<PlayerUpdate>(trade_deal->get_recipient(),
				trade_deal->get_sell_type(), trade_deal->get_sell_amount() + receiver_sell_amount);
			std::shared_ptr<PlayerUpdate> recipient_buy_update = std::make_shared<PlayerUpdate>(trade_deal->get_recipient(),
				trade_deal->get_buy_type(), receiver_buy_amount - trade_deal->get_buy_amount());

			this->addUpdateToSendQueue(sender_sell_update);
			this->addUpdateToSendQueue(recipient_sell_update);
			this->addUpdateToSendQueue(sender_buy_update);
			this->addUpdateToSendQueue(recipient_buy_update);
		}
		case TradeCommand::CMD_TRADE_DECLINE: {
			recipient->trade_deal_decline(command->initiator);

			// TODO: Let sender know that his proposal was denied QAQ

			break;
		}
		default:
			LOG_ERR("Invalid trade command.");
	}
}

void GameServer::handleTradeData(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<TradeData> deal) {
	LOG_DEBUG("Received a trade data.");

	if (players.find(deal->sender) == players.end() || players.find(deal->recipient) == players.end()) {
		LOG_ERR("Invalid player ID");
		return;
	}

	/* If receives a pending trade deal, change the ID of the other player to sender, and sent it to its recipient. */
	if (deal->trade_status == TradeData::PENDING) {
		/* Create the corresponding TradeDeal from TradeData */
		std::shared_ptr<TradeDeal> trade_deal = std::make_shared<TradeDeal>(deal);
		deal->trade_deal_id = trade_deal->getID(); // Assign the TradeDeal's ID

		/* Put the TradeDeal to recipient's pending deals */
		players[deal->recipient]->receive_trade_deal(trade_deal);
		
		SunNet::ChanneledSocketConnection_p recipient = (connections.first)[deal->recipient]; // find recipient by ID

		/* Send this deal to both sender and recipient. Sender thus knows about the trade deal's ID. */
		this->addUpdateToSendQueue(deal, { sender, recipient });
	}
}

void GameServer::subscribeToChannels() {
	/* Subscribe to important channels */
	this->subscribe<DebugPause>(std::bind(&GameServer::handleGamePause, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<PlayerClientToServerTransfer>(std::bind(&GameServer::handleReceivePlayerClientToServerTransfer, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<PlayerCommand>(std::bind(&GameServer::handlePlayerCommand, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<UnitCommand>(std::bind(&GameServer::handleUnitCommand, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<TradeCommand>(std::bind(&GameServer::handleTradeCommand, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<TradeData>(std::bind(&GameServer::handleTradeData, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<SettleCityCommand>(std::bind(&GameServer::handleSettleCityCommand, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<PlayerClientToServerTransfer>(std::bind(&GameServer::handleReceivePlayerClientToServerTransfer, this, std::placeholders::_1, std::placeholders::_2));
	this->subscribe<ResearchCommand>(std::bind(&GameServer::handleResearchCommand, this, std::placeholders::_1, std::placeholders::_2));
}
