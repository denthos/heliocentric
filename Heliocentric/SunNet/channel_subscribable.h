#pragma once

#include <functional>
#include <unordered_map>
#include <set>
#include "channels.h"
#include "socket_connection.hpp"


namespace Sunnet {
	typedef unsigned int SUBSCRIPTION_ID;

	template <class TSubscriptionType>
	void subscriptionCaster(NETWORK_BYTE* data, std::function<void(TSubscriptionType*)> handler) {
		TSubscriptionType* obj = reinterpret_cast<TSubscriptionType*>(data);
		handler(obj);
	}

	class ChannelSubscribable {
	private:
		static std::atomic<SUBSCRIPTION_ID> subscription_counter;
		std::unordered_map < CHANNEL_ID, std::set<SUBSCRIPTION_ID>> subscription_ids;
		std::unordered_map < SUBSCRIPTION_ID, std::function<void(NETWORK_BYTE*)>> subscriptions;
	public:
		void handleIncomingMessage(SocketConnection_p socket);

		template <class TSubscriptionType>
		SUBSCRIPTION_ID subscribe(std::function<void(TSubscriptionType*)> callback) {
			std::function<void(NETWORK_BYTE*)> callback_func = std::bind(subscriptionCaster<TSubscriptionType>, std::placeholders::_1, callback);
			CHANNEL_ID channel_id = Channels::types_to_ids[typeid(TSubscriptionType)];

			SUBSCRIPTION_ID sub_id = ChannelSubscribable::subscription_counter++;
			subscription_ids[channel_id].insert(sub_id);
			subscriptions[sub_id] = callback_func;

			return sub_id;
		}

	};
}