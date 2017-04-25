#pragma once

#include <functional>
#include <unordered_map>
#include <set>
#include "channels.h"
#include "socket_connection.hpp"
#include "channel_subscription.h"
#include "channeled_socket_connection.h"


namespace SunNet {
	class ChannelSubscribable {
	private:
		std::unordered_map < CHANNEL_ID, std::shared_ptr<ChannelSubscriptionInterface>> subscriptions;
	public:
		void handleIncomingMessage(ChanneledSocketConnection_p socket);

		template <class TSubscriptionType>
		SUBSCRIPTION_ID subscribe(std::function<void(ChanneledSocketConnection_p, std::shared_ptr<TSubscriptionType>)> callback) {
			CHANNEL_ID channel_id = Channels::getChannelId<TSubscriptionType>();
			std::shared_ptr<ChannelSubscriptionInterface> subscription;

			auto subscription_iter = this->subscriptions.find(channel_id);
			if (subscription_iter != this->subscriptions.end()) {
				subscription = subscription_iter->second;
			}
			else {
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

			if (typed_subscription->unsubscribe(id)) {
				this->subscriptions.erase(channel_id);
			}
		}

		virtual void handleSocketDisconnect(ChanneledSocketConnection_p socket) {}
	};
}
