#pragma once

#include "server.h"
#include "channel_subscribable.h"
#include "channeled_socket_connection.h"

namespace Sunnet {
	template <typename TSocketConnectionType>
	class ChanneledServer : public Server<TSocketConnectionType>, public ChannelSubscribable {
	public:

		template <class ... ArgType>
		ChanneledServer(std::string address, std::string port, int listen_queue_size, int poll_timeout, ArgType ... args) :
			Server(address, port, listen_queue_size, poll_timeout, args...) {}

		virtual void handle_ready_to_read(SocketConnection_p client) {
			ChanneledSocketConnection_p channeled_socket = (
				std::static_pointer_cast<ChanneledSocketConnection>(client)
				);
			this->handleIncomingMessage(channeled_socket);
		}

		virtual void handleSocketDisconnect(SocketConnection_p socket) {
			this->removeFromPollService(socket);
			this->handleClientDisconnect(socket);
		}

		virtual void handleClientDisconnect(SocketConnection_p client) {}
	};
}