#pragma once
#include "socketutil.h"

#include <map>
#include <typeindex>
#include <memory>
#include <atomic>

namespace Sunnet {
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
	public:
		static std::map<CHANNEL_ID, ChannelInterface*> ids_to_channels;
		static std::map<std::type_index, CHANNEL_ID> types_to_ids;
		static std::atomic<CHANNEL_ID> channel_counter;

		template <class TChannelType>
		static void addNewChannel() {
			ChannelInterface* channel = new Channel<TChannelType>();
			Channels::ids_to_channels[channel->getId()] = channel;
			Channels::types_to_ids[typeid(TChannelType)] = channel->getId();
		}
	};


	template <class TData>
	class Channel : public ChannelInterface {
	public:
		Channel() : ChannelInterface(sizeof(TData), Channels::channel_counter++) {}
	};
}