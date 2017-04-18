#pragma once

#include "socketutil.h"

#include <atomic>
#include <unordered_map>
#include <functional>
#include <memory>

namespace Sunnet {
	typedef unsigned int SUBSCRIPTION_ID;

	class ChannelSubscriptionInterface {
	public:
		virtual void propagate_to_handlers(std::shared_ptr<NETWORK_BYTE> data) {}
	};

	template <typename TSubscriptionType>
	class ChannelSubscription : public ChannelSubscriptionInterface {
	private:
		std::atomic<SUBSCRIPTION_ID> subscription_counter;
		std::unordered_map<SUBSCRIPTION_ID, std::function<void(std::shared_ptr<TSubscriptionType>)>> subscriptions;

	public:
		ChannelSubscription() : subscription_counter(0) {}

		SUBSCRIPTION_ID subscribe(std::function<void(std::shared_ptr<TSubscriptionType>)> handler) {
			SUBSCRIPTION_ID subscription_id = this->subscription_counter++;
			this->subscriptions[subscription_id] = handler;

			return subscription_id;
		}

		void unsubscribe(SUBSCRIPTION_ID id) {
			this->subscriptions.erase(id);
		}

		virtual void propagate_to_handlers(std::shared_ptr<NETWORK_BYTE> data) {
			auto p = reinterpret_cast<typename std::shared_ptr<TSubscriptionType>::element_type*>(data.get());
			std::shared_ptr<TSubscriptionType> obj(data, p);

			for (auto subscription : subscriptions) {
				subscription.second(obj);
			}
		}
	};
}