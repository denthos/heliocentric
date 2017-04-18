#include "tcp_socket_connection.hpp"
#include "client.h"
#include "server.h"
#include "channel_subscribable.h"

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

		bool open = client->receive(data, (NETWORK_BYTE_SIZE) message.size());
		std::cout << data << std::endl;

		client->send(data, (NETWORK_BYTE_SIZE) message.size());

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

		this->read(data, (NETWORK_BYTE_SIZE) message.size());
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

struct GeneralMessage {
	int msg;
};

void callback(std::shared_ptr<GeneralMessage> ptr) {
	std::cout << "SUBSCRIPTION CALLBACK" << std::endl;
	std::cout << "MESSAGE: " << ptr->msg << std::endl;
}

int main(int argc, char* argv[]) {

	TestServer server("0.0.0.0", "9876", 5);
	server.open();
	server.serve(1000 * 2);

	TestClient client(1000 * 2);
	client.connect("127.0.0.1", "9876");

	Channels::addNewChannel<GeneralMessage>();
	ChannelSubscribable sub;
	sub.subscribe<GeneralMessage>(callback);

	SocketConnection_p server_conn = std::make_shared<TCPSocketConnection>();
	SocketConnection_p client_conn = std::make_shared<TCPSocketConnection>();

	server_conn->bind("9870");
	server_conn->listen(5);

	NETWORK_BYTE channel = 0;

	struct GeneralMessage msg;
	msg.msg = 1337;

	client_conn->connect("127.0.0.1", "9870");
	SocketConnection_p client_server_conn = server_conn->accept();
	client_server_conn->send(&channel, sizeof(NETWORK_BYTE));
	client_server_conn->send((NETWORK_BYTE*)&msg, sizeof(GeneralMessage));

	sub.handleIncomingMessage(client_conn);

	std::string message = "Hello, world!";
	client.send(message.c_str(), (NETWORK_BYTE_SIZE) message.size());
	client.send(message.c_str(), (NETWORK_BYTE_SIZE) message.size());

	std::string dummy;
	std::getline(std::cin, dummy);
}
