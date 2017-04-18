#pragma once

#include "socketutil.h"
#include "channeled_socket_connection.h"

#include <atomic>
#include <unordered_map>
#include <functional>
#include <memory>

namespace Sunnet {
	typedef unsigned int SUBSCRIPTION_ID;

	class ChannelSubscriptionInterface {
	public:
		virtual void propagate_to_handlers(ChanneledSocketConnection_p sender, std::unique_ptr<NETWORK_BYTE[]> data) {}
	};

	template <typename TSubscriptionType>
	class ChannelSubscription : public ChannelSubscriptionInterface {
	private:
		std::atomic<SUBSCRIPTION_ID> subscription_counter;
		std::unordered_map<SUBSCRIPTION_ID, std::function<void(ChanneledSocketConnection_p, std::shared_ptr<TSubscriptionType>)>> subscriptions;

	public:
		ChannelSubscription() : subscription_counter(0) {}

		SUBSCRIPTION_ID subscribe(std::function<void(ChanneledSocketConnection_p, std::shared_ptr<TSubscriptionType>)> handler) {
			SUBSCRIPTION_ID subscription_id = this->subscription_counter++;
			this->subscriptions[subscription_id] = handler;

			return subscription_id;
		}

		bool unsubscribe(SUBSCRIPTION_ID id) {
			this->subscriptions.erase(id);
			return (this->subscriptions.size() == 0);
		}

		void propagate_to_handlers(ChanneledSocketConnection_p sender, std::unique_ptr<NETWORK_BYTE[]> data) {
			std::shared_ptr<NETWORK_BYTE> shared_data = std::move(data);

			auto p = reinterpret_cast<typename std::shared_ptr<TSubscriptionType>::element_type*>(shared_data.get());
			std::shared_ptr<TSubscriptionType> obj(shared_data, p);

			for (auto subscription : subscriptions) {
				subscription.second(sender, obj);
			}
		}
	};
}
