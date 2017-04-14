#include "tcp_socket_connection.hpp"
#include "client.h"
#include "server.h"

#include <memory>
#include <iostream>
#include <string>

using namespace Sunnet;

class TestServer : public Server<TCPSocketConnection> {
public:
	TestServer(std::string address, std::string port, int queue_size) :
		Server<TCPSocketConnection>(address, port, queue_size, 1000 * 2) {}

	void handle_client_connect(SocketConnection_p client) {
		std::cout << "Client connected!" << std::endl;
	}

	void handle_ready_to_read(SocketConnection_p client) {
		std::cout << "Client sent data!" << std::endl;

		char data[512];
		std::memset(data, 0, 512);
		std::string message = "Hello, world!";

		bool open = client->receive(data, message.size());
		std::cout << data << std::endl;

		client->send(data, message.size());

		if (!open) {
			std::cout << "Client disconnect" << std::endl;
			this->remove_socket(client);
		}
	}

	void handle_poll_timeout() {
		std::cout << "Poll timeout!" << std::endl;
	}
};

class TestClient : public Client<TCPSocketConnection> {
public:
	TestClient(int poll_timeout) : Client<TCPSocketConnection>(poll_timeout) {}

	void handle_client_ready_to_read() {
		std::cout << "Client ready to read!" << std::endl;
		char data[512];
		std::memset(data, 0, 512);
		std::string message = "Hello, world!";

		this->read(data, message.size());
		std::cout << data << std::endl;
	}

	void handle_poll_timeout() {
		std::cout << "Client poll timeout" << std::endl;
	}

	void handle_connection_disconnect() {
		std::cout << "Client connection disconnect" << std::endl;
	}

	void handle_client_error() {
		std::cout << "CLIENT ERROR!" << std::endl;
	}
};

int main(int argc, char* argv[]) {

	TestServer server("0.0.0.0", "9876", 5);
	server.open();
	server.serve(1000 * 2);

	TestClient client(1000 * 2);
	client.connect("127.0.0.1", "9876");

	std::string message = "Hello, world!";
	client.send(message.c_str(), message.size());
	client.send(message.c_str(), message.size());

	std::string dummy;
	std::getline(std::cin, dummy);
}
