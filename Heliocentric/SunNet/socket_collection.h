#pragma once
#include "socket_connection.hpp"
#include <unordered_set>

namespace Sunnet {
	typedef std::unordered_set<SocketConnection_p, SocketConnection_p_hash> SocketCollection;
	typedef std::shared_ptr<SocketCollection> SocketCollection_p;

	struct SocketConnection_p_hash {
		std::size_t operator()(const SocketConnection_p& connection_ptr) const {
			return std::hash<SOCKET>()(connection_ptr->socket_descriptor);
		}
	};
}