#pragma once
#include "socket_collection.h"
#include "pollservice.h"

#include <thread>
#include <mutex>
#include <functional>

namespace SunNet {
	/**
	A collection of states that the server can be in

	  +--------+        +------+         +-------+        +-------------+
	->| CLOSED |  ----> | OPEN |  ---->  | SERVE | ---->  | DESTRUCTING |
	  +--------+        +------+         +-------+        +-------------+
          ^                |                 |
		  +----------------+-----------------+
	*/
	enum ServerState {
		DESTRUCTING, /** < The server is closed. It has no socket associated with it */
		CLOSED, /** < The server was created and is ready to be opened */

		OPEN, /** < The server has been binded and listening, but not yet polling */
		SERVE /** < The server is actively polling and accepting */
	};

	/**
	A class which represents a server, which clients can connect to. The server continuously
	polls connected clients and calls hooks, which inheritors can implement to their liking.

	The server operates in two-threads - A main thread and a thread for polling. This makes
	things tricky. Try not to touch the internals of the server unless you want to deal with
	possible race conditions!
	*/
	template <class TSocketConnection>
	class Server {
	private:
		SocketConnection_p server_connection;

		PollService poll_service;
		std::thread poll_thread;

		/* We are going to edit the poll service while its running. */
		std::mutex poll_service_mutex;

		std::string address;
		std::string port;
		int listen_queue_size;
		int poll_timeout;
		ServerState state;

		void state_transition(std::initializer_list<ServerState> const & valid_from_states, ServerState new_state) {
			bool in_valid_from_state = false;
			for (const ServerState& valid_from_state : valid_from_states) {
				if (this->state == valid_from_state) {
					in_valid_from_state = true;
					break;
				}
			}

			if (!in_valid_from_state) {
				throw InvalidStateTransitionException();
			}

			this->state = new_state;
		}

		/* 
		Used to hold a function to create the connection. This is populated on
		construction and used to re-open connections
		*/
		std::function<SocketConnection_p()> connection_create_func;


		/**
		The polling function to be executed by the polling thread. It polls all
		connected clients and calls hooks depending on the status of the
		clients
		*/
		void poll() {
			while (this->state == SERVE) {
				SocketCollection_p ready_sockets;
				{
					std::lock_guard<std::mutex> lock(this->poll_service_mutex);
					ready_sockets = poll_service.poll();
				}

				if (ready_sockets->size() == 0) {
					/* CHECKPOINT */
					if (this->state == DESTRUCTING) break;
					this->handle_poll_timeout();
					continue;
				}

				for (auto socket_it = ready_sockets->begin(); socket_it != ready_sockets->end(); ++socket_it) {
					if (*socket_it == this->server_connection) {
						if ((*socket_it)->has_error()) {
							/* CHECKPOINT */
							if (this->state == DESTRUCTING) break;
							this->handle_server_connection_error();
						}
						else {
							/* CHECKPOINT */
							if (this->state == DESTRUCTING) break;
							this->handle_connection_request();
						}
					}
					else {
						if ((*socket_it)->has_error()) {
							/* CHECKPOINT */
							if (this->state == DESTRUCTING) break;
							this->handle_client_error(*socket_it);
						}
						else{
							/* CHECKPOINT */
							if (this->state == DESTRUCTING) break;
							this->handle_ready_to_read(*socket_it);
						}
					}
				}
			}
		}


	protected:
		/**
		Adds a socket to the poll service, esp. useful when a new client connects.
		*/
		void addToPollService(SocketConnection_p socket) {
			std::lock_guard<std::mutex> lock(this->poll_service_mutex);
			this->poll_service.add_socket(socket);
		}

		/**
		Removes a socket from the poll service, esp. useful when a client disconnects */
		void removeFromPollService(SocketConnection_p socket) {
			std::lock_guard<std::mutex> lock(this->poll_service_mutex);
			this->poll_service.remove_socket(socket);
		}

		void clearPollService() {
			std::lock_guard<std::mutex> lock(this->poll_service_mutex);
			this->poll_service.clear_sockets();
		}

		/**
		The default implementation for when a new connection request comes in.
		This can be overriden by users, but chances are they'd like to perform
		the default behavior, which simply adds the connection to the
		poll service and delegates to another "hook"
		*/
		virtual void handle_connection_request() {
			SocketConnection_p new_client = this->server_connection->accept();

			this->addToPollService(new_client);
			this->handle_client_connect(new_client);
		}

		/* Handlers for an inheritor to implement */
		virtual void handle_server_connection_error() = 0;
		virtual void handle_client_error(SocketConnection_p client) = 0;
		virtual void handle_client_connect(SocketConnection_p client) = 0;
		virtual void handle_ready_to_read(SocketConnection_p client) = 0;
		virtual void handle_poll_timeout() = 0;

	public:
		template <class ... ArgType>
		Server(std::string address, std::string port, int listen_queue_size, int poll_timeout, ArgType ... args) : 
			address(address), port(port), listen_queue_size(listen_queue_size), poll_timeout(poll_timeout), state(CLOSED) {

			/* Bind the template arguments to a function we can use to re-create the connection */
			this->connection_create_func = []() { return std::make_shared<TSocketConnection>(args...);  };
			this->poll_service = PollService(poll_timeout);
		}

		/**
		Destruct the server, which does the following:
		1. Change state, if necessary
		2. Join the polling thread, if necessary
		3. Close the server connection
		*/
		virtual ~Server() {
			/*
			We are going to FORCE a state change into the destructing state.
			If the server is being destructed without calling close() first, then we
			are in an EMERGENCY SITUATION.

			We are switching out of the "SERVE" state in order to get the polling thread to
			stop, but if it calls any virtual functions when we join a few lines down, we
			are boned. C++ will except us. So let's pray to God that this won't happen and the
			function will instead hit a checkpoint.
			*/
			this->state = DESTRUCTING;


			/* Join the poll thread if necessary. Cross our fingers that no virtual functions are called. */
			if (this->poll_thread.joinable()) {
				this->poll_thread.join();
			}

			/* AXE THE CONNECTION, MY LORD! */
			this->server_connection.reset();
		}

		/**
		Opens the server by binding and listening on the given port and
		address.
		*/
		void open() {
			this->state_transition({ CLOSED }, OPEN);

			/* Create the connection */
			this->server_connection = this->connection_create_func();
			this->poll_service.add_socket(this->server_connection);

			/* Attempt to bind and listen. Change state if both succeed */
			this->server_connection->bind(this->port, this->address);
			this->server_connection->listen(this->listen_queue_size);

			this->state = OPEN;
		}

		/**
		Puts the server in "serve" mode, which means that the server will spin
		up a polling thread and begin accepting new connections and firing hooks
		for existing ones.
		*/
		void serve() {
			this->state_transition({ OPEN }, SERVE);
			this->poll_thread = std::thread(&Server<TSocketConnection>::poll, this);
		}


		/**
		Attempts to forcibly close the server.
		*/
		void close() {
			/* Force a closing state change, which will stop the poll thread when its ready */
			this->state_transition({ SERVE, OPEN }, CLOSED);

			/* 
			It is possible that close() is being called from the poll_thread. 
			We should only join if that is _not_ the case.

			Also no rush. The polling thread will not bail out at any checkpoints.
			The user is properly closing us, so let's take our sweet time :)
			*/
			if (std::this_thread::get_id() != this->poll_thread.get_id()) {
				if (this->poll_thread.joinable()) {
					this->poll_thread.join();
				}
			}

			this->clearPollService();

			/* KILL THE CONNECTION! */
			this->server_connection.reset();
		}


		
		class ServerException : public std::exception {};
		class InvalidStateTransitionException : public ServerException {};
	};

}
