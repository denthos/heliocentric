#include "channel_subscribable.h"

namespace Sunnet {
	void ChannelSubscribable::handleIncomingMessage(SocketConnection_p socket) {
			CHANNEL_ID channel_id;
			socket->receive(&channel_id, sizeof(CHANNEL_ID));
			std::shared_ptr<ChannelInterface> channel = Channels::getChannel(channel_id);

			std::shared_ptr<NETWORK_BYTE> data(new NETWORK_BYTE[channel->getMessageSize()], std::default_delete<NETWORK_BYTE[]>());
			socket->receive(data.get(), channel->getMessageSize());

			for (auto subscription : subscriptions) {
				subscription.second->propagate_to_handlers(data);
			}
	}
}