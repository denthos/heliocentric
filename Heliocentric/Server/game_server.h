#pragma once
#include <queue>
#include <memory>

#include "locked_item.h"

#include "player.h"
#include "identifiable.h"
#include "channeled_server.h"
#include "tcp_socket_connection.hpp"
#include "player_update.h"
#include "game_object_update.h"

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
	std::unordered_map<UID, std::shared_ptr<SunNet::ChanneledSocketConnection_p>> player_connections;
	std::unordered_map<UID, std::unique_ptr<Player>> players;
	std::unordered_map<UID, std::unique_ptr<GameObject>> game_objects;

	Lib::LockedItem < std::queue<std::shared_ptr<GameObjectUpdate>>> object_updates;
	Lib::LockedItem < std::queue<std::shared_ptr<PlayerUpdate>>> player_updates;
};