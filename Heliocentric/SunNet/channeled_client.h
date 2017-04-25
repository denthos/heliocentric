#pragma once

#include "client.h"
#include "channel_subscribable.h"
#include "channeled_socket_connection.h"

namespace SunNet {

	template <typename TSocketConnection>
	class ChanneledClient : public Client<TSocketConnection>, public ChannelSubscribable {
	private:
		ChanneledSocketConnection_p channeled_connection;

	protected:
		virtual void handleSocketDisconnect(ChanneledSocketConnection_p socket) {
			this->handle_client_disconnect();
		}

		virtual void handle_client_ready_to_read() {
			/* Delegate to ChannelSubscribable */
			this->handleIncomingMessage(this->channeled_connection);
		}

		/**** Handlers for ChanneledClient ****/
		virtual void handle_client_disconnect() = 0;
		virtual void handle_client_error() = 0;
		virtual void handle_poll_timeout() = 0;

	public:
		template <class ... ArgTypes>
		ChanneledClient(int poll_timeout, ArgTypes ... args) : Client(poll_timeout, args...) {
			this->channeled_connection = std::static_pointer_cast<ChanneledSocketConnection>(this->connection);
		}

		template <class TMessageType>
		void channeled_send(TMessageType* message) {
			this->channeled_connection->channeled_send<TMessageType>(message);
		}

		CHANNEL_ID channeled_read_id() {
			return this->channeled_connection->channeled_read_id();
		}

		std::unique_ptr<NETWORK_BYTE[]> channeled_read(CHANNEL_ID id) {
			return this->channeled_connection->channeled_read(id);
		}
	};
}
