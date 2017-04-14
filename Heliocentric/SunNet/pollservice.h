/**
@file pollservice.h
@brief Definitions for PollService, a class which polls a collection of
sockets and allows action on ones which are ready to read
*/
#pragma once

#include "socket_connection.hpp"
#include "socket_collection.h"
#include <functional>
#include <vector>

namespace Sunnet {

	/**
	A C++ wrapper class around WinSock/Sockets poll().

	Keeps track of a collection of SocketConnection objects and allows
	operation when they are ready to be read.
	*/
	class PollService {
	private:
		int timeout; /** < How long to wait before declaring no socket can be read */
		SocketCollection_p sockets; /** < The collection of sockets to watch */
		std::vector<POLL_DESCRIPTOR> descriptors; /** < The corresponding poll() descriptors */

		SocketCollection_p results;

	public:
		/**
		Create a PollService object from a collection of sockets.

		@param sockets The sockets to monitor
		@param timeout how long to wait before declaring no socket can be read (in ms)
		*/
		PollService(const SocketCollection_p sockets, int timeout = 10);

		/**
		Adds a collection of sockets to the poll's collection of sockets.

		@param sockets The sockets to add
		*/
		void add_sockets(const SocketCollection_p sockets);

		/**
		Poll all sockets, returning those ready to be read

		@throws PollException if there was an error with the OS level poll()
		@throws PollReturnEventException if a specific socket encountered an error
		@throws InvalidSocketConnectionException if a watched socket's 
		corresponding descriptor could not be found
		@return A collection of sockets ready to be read
		*/
		SocketCollection_p poll();
	};

	class PollException : public SocketException {
	public:
		PollException(std::string msg) : SocketException(msg) {};
	};

	class InvalidSocketConnectionException : public PollException {
	private:
		SOCKET descriptor;
	public:
		InvalidSocketConnectionException(std::string msg, SOCKET descriptor) : PollException(msg) {
			this->descriptor = descriptor;
		}

		SOCKET get_descriptor() {
			return this->descriptor;
		}
	};

	class PollReturnEventException : public PollException {
	private:
		SocketConnection_p socket;
	public:
		PollReturnEventException(std::string msg, SocketConnection_p socket) : PollException(msg) {
			this->socket = socket;
		}

		SocketConnection_p get_socket() {
			return this->socket;
		}
	};
}

