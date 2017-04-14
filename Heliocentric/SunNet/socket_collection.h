#pragma once
#include "socket_connection.hpp"
#include <map>
#include <set>
#include <iterator>

namespace Sunnet {
	typedef std::set<SocketConnection_p>::iterator Socket_iter;
	typedef std::set<SocketConnection_p>::const_iterator Socket_const_iter;

	class SocketCollection {
		friend class PollService;


	private:
		std::set<SocketConnection_p> sockets;
		std::map<SOCKET, SocketConnection_p> descriptor_map;

		SocketConnection_p get_by_descriptor(SOCKET descriptor);

	public:
		SocketCollection();
		SocketCollection(SocketConnection_p socket);

		template <typename Iter>
		SocketCollection(Iter sockets_begin, Iter sockets_end) {
			for (Iter it = sockets_begin; it != sockets_end; ++it) {
				SocketConnection_p socket = *it;
				this->add(socket);
			}
		}

		Socket_iter begin();
		Socket_iter end();

		Socket_const_iter cbegin();
		Socket_const_iter cend();

		void clear();
		void add(SocketConnection_p socket);
	};

	typedef std::shared_ptr<SocketCollection> SocketCollection_p;
}
