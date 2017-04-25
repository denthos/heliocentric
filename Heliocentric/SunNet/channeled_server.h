#pragma once

#include "server.h"
#include "channel_subscribable.h"
#include "channeled_socket_connection.h"

namespace SunNet {
	template <typename TSocketConnectionType>
	class ChanneledServer : public Server<TSocketConnectionType>, public ChannelSubscribable {
	protected:
		virtual void handle_ready_to_read(SocketConnection_p client) {
			ChanneledSocketConnection_p channeled_socket = (
				std::static_pointer_cast<ChanneledSocketConnection>(client)
			);

			/* Delegate to ChannelSubscribable for parsing */
			this->handleIncomingMessage(channeled_socket);
		}

		virtual void handleSocketDisconnect(ChanneledSocketConnection_p socket) {
			this->removeFromPollService(socket);
			this->handleClientDisconnect(socket);
		}

		virtual void handle_client_error(SocketConnection_p client) {
			handle_channeledclient_error(std::static_pointer_cast<ChanneledSocketConnection>(client));
		}


		virtual void handle_client_connect(SocketConnection_p client) {
			handle_channeledclient_connect(std::static_pointer_cast<ChanneledSocketConnection>(client));
		}

		/**** Handlers for ChanneledServer ****/
		virtual void handleClientDisconnect(ChanneledSocketConnection_p client) = 0;

		/**** Handlers for server class ****/
		virtual void handle_server_connection_error() = 0;
		virtual void handle_channeledclient_error(ChanneledSocketConnection_p client) = 0;
		virtual void handle_channeledclient_connect(ChanneledSocketConnection_p client) = 0;
		virtual void handle_poll_timeout() = 0;

	public:

		template <class ... ArgType>
		ChanneledServer(std::string address, std::string port, int listen_queue_size, int poll_timeout, ArgType ... args) :
			Server(address, port, listen_queue_size, poll_timeout, args...) {}
	};
}
