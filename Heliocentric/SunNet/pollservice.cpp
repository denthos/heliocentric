#include "pollservice.h"

#include <string>


namespace Sunnet {
	PollService::PollService(const SocketCollection_p sockets, int timeout) : timeout(timeout) {
		this->sockets = std::make_shared<SocketCollection>();
		this->results = std::make_shared<SocketCollection>();
		this->add_sockets(sockets);
	}

	void PollService::add_sockets(const SocketCollection_p sockets) {
		for (auto it = sockets->descriptor_map.begin(); it != sockets->descriptor_map.end(); ++it) {
			SOCKET sock_descriptor = it->first;
			SocketConnection_p sock_connection = it->second;

			POLL_DESCRIPTOR poll_descriptor;
			poll_descriptor.events = POLLIN;
			poll_descriptor.fd = sock_descriptor;

			this->descriptors.push_back(poll_descriptor);
			this->sockets->add(sock_connection);
		}
	}

	SocketCollection_p PollService::poll() {
		this->results->clear();
		int poll_return = socket_poll(this->descriptors.data(), (NUM_POLL_DESCRIPTORS) this->descriptors.size(), this->timeout);

		if (poll_return == SOCKET_ERROR) {
			throw PollException(std::to_string(get_previous_error_code()));
		}
		else if (poll_return > 0) {
			for (auto poll_iter = this->descriptors.begin(); poll_iter != this->descriptors.end(); ++poll_iter) {
				if (poll_iter->revents & (POLLIN | POLLERR | POLLNVAL | POLLHUP)) {
					SocketConnection_p ready_socket = this->sockets->get_by_descriptor(poll_iter->fd);

					if (!ready_socket) {
						throw InvalidSocketConnectionException("Invalid socket descriptor", poll_iter->fd);
					}

					if (poll_iter->revents & (POLLERR | POLLNVAL | POLLHUP)) {
						throw PollReturnEventException(std::to_string(poll_iter->revents), ready_socket);
					}

					this->results->add(ready_socket);
				}
			}
		}

		return this->results;
	}
}
