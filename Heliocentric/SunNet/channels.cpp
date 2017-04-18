#include "channels.h"

namespace Sunnet {
	std::map<CHANNEL_ID, ChannelInterface*> Channels::ids_to_channels;
	std::map<std::type_index, CHANNEL_ID> Channels::types_to_ids;
	std::atomic<CHANNEL_ID> Channels::channel_counter = 0;

	ChannelInterface::ChannelInterface(NETWORK_BYTE_SIZE size, CHANNEL_ID id) :
		message_size(size), channel_id(id) {}
}
