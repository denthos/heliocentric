#pragma once
#include "socketutil.h"

#include <map>
#include <typeindex>
#include <memory>
#include <atomic>

namespace SunNet {
	typedef NETWORK_BYTE CHANNEL_ID;

	class ChannelInterface {
	private:
		NETWORK_BYTE_SIZE message_size;
		CHANNEL_ID channel_id;
	public:
		ChannelInterface(NETWORK_BYTE_SIZE size, CHANNEL_ID id);
		NETWORK_BYTE_SIZE getMessageSize() { return this->message_size; }
		CHANNEL_ID getId() { return this->channel_id; }
	};

	class Channels {
	private:
		static std::map<CHANNEL_ID, std::shared_ptr<ChannelInterface>> ids_to_channels;
		static std::map<std::type_index, CHANNEL_ID> types_to_ids;
		static std::atomic<CHANNEL_ID> channel_counter;
	public:

		template <class TChannelType>
		static CHANNEL_ID getChannelId() {
			return types_to_ids.at(typeid(TChannelType));
		}

		static std::shared_ptr<ChannelInterface> getChannel(CHANNEL_ID id);

		static CHANNEL_ID getNextId() { return channel_counter++; }

		template <class TChannelType>
		static void addNewChannel() {
			std::shared_ptr<ChannelInterface> channel = std::make_shared<Channel<TChannelType>>();
			Channels::ids_to_channels[channel->getId()] = channel;
			Channels::types_to_ids[typeid(TChannelType)] = channel->getId();
		}
	};


	template <class TData>
	class Channel : public ChannelInterface {
	public:
		Channel() : ChannelInterface(sizeof(TData), Channels::getNextId()) {}
	};
}