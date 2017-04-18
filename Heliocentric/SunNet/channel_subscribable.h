#pragma once

#include <functional>
#include <unordered_map>
#include <set>
#include "channels.h"
#include "socket_connection.hpp"
#include "channel_subscription.h"


namespace Sunnet {
	class ChannelSubscribable {
	private:
		std::unordered_map < CHANNEL_ID, std::shared_ptr<ChannelSubscriptionInterface>> subscriptions;
	public:
		void handleIncomingMessage(SocketConnection_p socket);

		template <class TSubscriptionType>
		SUBSCRIPTION_ID subscribe(std::function<void(std::shared_ptr<TSubscriptionType>)> callback) {
			CHANNEL_ID channel_id = Channels::getChannelId<TSubscriptionType>();
			std::shared_ptr<ChannelSubscriptionInterface> subscription;

			try {
				subscription = this->subscriptions.at(channel_id);
			}
			catch (std::out_of_range&) {
				subscription = std::make_shared<ChannelSubscription<TSubscriptionType>>();
				this->subscriptions[channel_id] = subscription;
			}

			std::shared_ptr<ChannelSubscription<TSubscriptionType>> typed_subscription = (
				std::static_pointer_cast<ChannelSubscription<TSubscriptionType>>(subscription)
			);

			return typed_subscription->subscribe(callback);
		}

		template <class TSubscriptionType>
		void unsubscribe(SUBSCRIPTION_ID id) {
			CHANNEL_ID channel_id = Channels::getChannelId<TSubscriptionType>();
			std::shared_ptr<ChannelSubscriptionInterface> subscription = this->subscriptions.at(channel_id);

			std::shared_ptr<ChannelSubscription<TSubscriptionType>> typed_subscription = (
				std::static_pointer_cast<ChannelSubscription<TSubscriptionType>>(subscription);
			);

			typed_subscription->unsubscribe(id);
		}

	};
}