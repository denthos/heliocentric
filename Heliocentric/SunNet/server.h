#pragma once
#include "socket_collection.h"
#include "pollservice.h"

#include <thread>
#include <mutex>

namespace SunNet {
	enum ServerState {
		INIT,
		OPEN,
		SERVE
	};

	template <class TSocketConnection>
	class Server {
	private:
		SocketConnection_p server_connection;

		PollService poll_service;
		std::thread poll_thread;
		std::mutex poll_service_mutex;

		std::string address;
		std::string port;
		int listen_queue_size;
		int poll_timeout;
		ServerState state;

		void state_transition(ServerState from, ServerState to) {
			if (this->state != from) {
				throw InvalidStateTransitionException();
			}
			this->state = to;
		}

	protected:
		void addToPollService(SocketConnection_p socket) {
			std::lock_guard<std::mutex> lock(this->poll_service_mutex);
			this->poll_service.add_socket(socket);
		}

		void removeFromPollService(SocketConnection_p socket) {
			std::lock_guard<std::mutex> lock(this->poll_service_mutex);
			this->poll_service.remove_socket(socket);
		}

	public:
		template <class ... ArgType>
		Server(std::string address, std::string port, int listen_queue_size, int poll_timeout, ArgType ... args) : 
			address(address), port(port), listen_queue_size(listen_queue_size), poll_timeout(poll_timeout), state(INIT) {

			this->server_connection = std::make_shared<TSocketConnection>(args...);
			this->poll_service = PollService(poll_timeout);

			this->poll_service.add_socket(this->server_connection);
		}

		virtual ~Server() {
			/* Only formally close if we are serving. */
			if (this->state == SERVE) {
				this->close();
			}

			/* Otherwise, just reset the socket */
			else {
				this->server_connection.reset();
			}
		}

		void open() {
			/* Attempt to bind and listen. Change state if both succeed */
			this->server_connection->bind(this->port, this->address);
			this->server_connection->listen(this->listen_queue_size);

			this->state_transition(INIT, OPEN);
		}

		void serve(int timeout_in_ms) {
			this->state_transition(OPEN, SERVE);
			this->poll_thread = std::thread(&Server<TSocketConnection>::poll, this);
		}

		void poll() {
			while (this->state == SERVE) {
				std::lock_guard<std::mutex> lock(this->poll_service_mutex);

				SocketCollection_p ready_sockets = poll_service.poll();

				if (ready_sockets->size() == 0) {
					this->handle_poll_timeout();
					continue;
				}

				for (auto socket_it = ready_sockets->begin(); socket_it != ready_sockets->end(); ++socket_it) {
					if (*socket_it == this->server_connection) {
						this->handle_connection_request();
					}
					else {
						this->handle_ready_to_read(*socket_it);
					}
				}
			}
		}

		void close() {
			this->state_transition(SERVE, INIT);
			this->server_connection.reset();
			this->poll_thread.join();
		}

		void remove_socket(SocketConnection_p socket) {
			this->poll_service.remove_socket(socket);
		}

		virtual void handle_connection_request() {
			SocketConnection_p new_client = this->server_connection->accept();

			this->poll_service.add_socket(new_client);
			this->handle_client_connect(new_client);
		}

		virtual void handle_client_connect(SocketConnection_p client) {}
		virtual void handle_ready_to_read(SocketConnection_p client) {}
		virtual void handle_poll_timeout() {}

		class ServerException : public std::exception {};
		class InvalidStateTransitionException : public ServerException {};
	};

}