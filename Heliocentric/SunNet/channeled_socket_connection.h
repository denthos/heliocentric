#pragma once
#include "socket_connection.hpp"
#include "channels.h"

namespace Sunnet {
	class ChanneledSocketConnection : public SocketConnection {
	public:
		ChanneledSocketConnection(int domain, int type, int protocol) :
			SocketConnection(domain, type, protocol) {}

		ChanneledSocketConnection(SOCKET socket_fd, int domain, int type, int protocol) :
			SocketConnection(socket_fd, domain, type, protocol) {}

		template <typename TMessageType>
		void channeled_send(TMessageType* message) {
			CHANNEL_ID channel_id = Channels::getChannelId<TMessageType>();

			this->send((NETWORK_BYTE*)&channel_id, sizeof(CHANNEL_ID));
			this->send((NETWORK_BYTE*)message, sizeof(TMessageType));
		}
		
		CHANNEL_ID channeled_read_id() {
			CHANNEL_ID channel_id;

			if (!this->receive(&channel_id, sizeof(CHANNEL_ID))) {
				throw ConnectionClosedException();
			}

			return channel_id;
		}

		template <typename TMessageType>
		std::unique_ptr<NETWORK_BYTE[]> channeled_read() {
			return this->read(Channels::getChannelId<TMessageType>());
		}

		std::unique_ptr<NETWORK_BYTE[]> channeled_read(CHANNEL_ID id) {
			std::shared_ptr<ChannelInterface> channel = Channels::getChannel(id);
			std::unique_ptr<NETWORK_BYTE[]> data = std::make_unique<NETWORK_BYTE[]>(channel->getMessageSize());

			if (!this->receive(data.get(), sizeof(channel->getMessageSize()))) {
				throw ConnectionClosedException();
			}

			return data;
		}

		class ConnectionClosedException : std::exception {};
	};

	typedef std::shared_ptr<ChanneledSocketConnection> ChanneledSocketConnection_p;
}
