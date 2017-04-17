#include "channel_subscribable.h"

namespace Sunnet {
	std::atomic<SUBSCRIPTION_ID> ChannelSubscribable::subscription_counter = 0;

	void ChannelSubscribable::handleIncomingMessage(SocketConnection_p socket) {
			CHANNEL_ID channel_id;
			socket->receive(&channel_id, sizeof(CHANNEL_ID));
			ChannelInterface* channel = Channels::ids_to_channels[channel_id];

			std::vector<NETWORK_BYTE> data(channel->getMessageSize());
			socket->receive(&data[0], channel->getMessageSize());

			for (auto subscription_id : subscription_ids[channel_id]) {
				subscriptions[subscription_id](&data[0]);
			}
	}
}