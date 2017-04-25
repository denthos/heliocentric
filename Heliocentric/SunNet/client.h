#pragma once
#include "socket_connection.hpp"
#include "pollservice.h"
#include <vector>
#include <thread>

namespace SunNet {
	enum ClientState {
		CLIENT_INIT,
		CLIENT_CONNECTED
	};

	template <class TSocketConnection>
	class Client {
	private:
		std::thread poll_thread;
		PollService poll_service;

		ClientState state;

		void state_transition(ClientState from, ClientState to) {
			if (this->state != from) {
				throw InvalidStateTransitionException();
			}

			this->state = to;
		}

	protected:
		std::shared_ptr<SocketConnection> connection;

	public:
		template <class ... ArgTypes>
		Client(int poll_timeout, ArgTypes ... args) : state(CLIENT_INIT) {
			this->connection = std::make_shared<TSocketConnection>(args...);
			this->poll_service = PollService(poll_timeout);
		}

		virtual ~Client() {
			/* Only formally disconnect if we are connected */
			if (this->state == CLIENT_CONNECTED) {
				this->disconnect();
			}

			/* Otherwise, just reset the connection. */
			else {
				this->connection.reset();
			}
		}

		void connect(std::string address, std::string port) {
			/* Connect. If the connection succeeds, change state */
			this->connection->connect(address, port);

			this->state_transition(CLIENT_INIT, CLIENT_CONNECTED);
			this->poll_service.add_socket(this->connection);
			this->poll_thread = std::thread(&Client<TSocketConnection>::poll, this);
		}

		void disconnect() {
			this->state_transition(CLIENT_CONNECTED, CLIENT_INIT);
			this->connection.reset();
			this->poll_thread.join();
		}

		void send(const NETWORK_BYTE* buffer, NETWORK_BYTE_SIZE size) {
			this->connection->send(buffer, size);
		}

		void read(NETWORK_BYTE* buffer, NETWORK_BYTE_SIZE size) {
			this->connection->receive(buffer, size);
		}

		void poll() {
			while (this->state == CLIENT_CONNECTED) {
				SocketCollection_p ready_sockets = this->poll_service.poll();

				if (ready_sockets->size() == 0) {
					this->handle_poll_timeout();
					continue;
				}
				
				for (auto socket_iter = ready_sockets->begin(); socket_iter != ready_sockets->end(); ++socket_iter) {
					if (*socket_iter == this->connection) {
						if ((*socket_iter)->has_error()) {
							this->handle_client_error();
						}
						else {
							this->handle_client_ready_to_read();
						}
					}
					else {
						throw InvalidSocketPollException();
					}
				}
			}
		}

		virtual void handle_client_error() {}
		virtual void handle_client_ready_to_read() {}
		virtual void handle_poll_timeout() {}

		class ClientException : public std::exception {};
		class InvalidStateTransitionException : public ClientException {};
		class InvalidSocketPollException : public ClientException {};
	};
}