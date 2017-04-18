#include "pollservice.h"

#include <string>


namespace Sunnet {

	PollService::PollService(int timeout) : timeout(timeout) {
		this->results = std::make_shared<SocketCollection>();
	}

	PollService::PollService(const SocketConnection_p socket, int timeout) : PollService(timeout) {
		this->add_socket(socket);
	}

	void PollService::add_socket(const SocketConnection_p socket) {
		POLL_DESCRIPTOR poll_descriptor;
		poll_descriptor.events = POLLIN;
		poll_descriptor.fd = socket->socket_descriptor;

		this->descriptors.push_back(poll_descriptor);
		this->poll_descriptor_map[socket->socket_descriptor] = std::make_pair(this->descriptors.size() - 1, socket);
	}

	void PollService::remove_socket(const SocketConnection_p socket) {
		std::pair<int, SocketConnection_p> info = this->poll_descriptor_map.at(socket->socket_descriptor);
		int index = info.first;

		this->descriptors.erase(this->descriptors.begin() + index);
		this->poll_descriptor_map.erase(socket->socket_descriptor);
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
					std::pair<int, SocketConnection_p> socket_info = this->poll_descriptor_map.at(poll_iter->fd);
					SocketConnection_p ready_socket = socket_info.second;

					if (!ready_socket) {
						throw InvalidSocketConnectionException("Invalid socket descriptor", poll_iter->fd);
					}

					if (poll_iter->revents & (POLLERR | POLLNVAL | POLLHUP)) {
						ready_socket->set_error();
					}

					this->results->insert(ready_socket);
				}
			}
		}

		return this->results;
	}
}
