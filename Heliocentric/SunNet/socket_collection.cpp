#include "socket_collection.h"

namespace Sunnet {
	SocketConnection_p SocketCollection::get_by_descriptor(SOCKET socket) {
		try {
			return this->descriptor_map.at(socket);
		}
		catch (const std::out_of_range& e) {
			return nullptr;
		}
	}

	SocketCollection::SocketCollection() {}

	SocketCollection::SocketCollection(SocketConnection_p socket) {
		this->sockets.insert(socket);
		this->descriptor_map[socket->socket_descriptor] = socket;
	}

	Socket_iter SocketCollection::begin() {
		return this->sockets.begin();
	}

	Socket_iter SocketCollection::end() {
		return this->sockets.end();
	}

	Socket_const_iter SocketCollection::cbegin() {
		return this->sockets.cbegin();
	}

	Socket_const_iter SocketCollection::cend() {
		return this->sockets.cend();
	}

	void SocketCollection::clear() {
		this->sockets.clear();
		this->descriptor_map.clear();
	}

	void SocketCollection::add(SocketConnection_p socket) {
		this->sockets.insert(socket);
		this->descriptor_map[socket->socket_descriptor] = socket;
	}
}