#include "tcp_socket_connection.hpp"
#include "channeled_client.h"
#include "channeled_server.h"

#include <memory>
#include <iostream>
#include <string>

class TestServer : public SunNet::ChanneledServer<SunNet::TCPSocketConnection> {
public:
	TestServer(std::string address, std::string port, int queue_size) :
		ChanneledServer<SunNet::TCPSocketConnection>(address, port, queue_size, 1000 * 2) {}

	void handle_client_connect(SunNet::SocketConnection_p client) {
		std::cout << "Client connected!" << std::endl;
	}

	void handle_poll_timeout() {
		std::cout << "Server poll timeout!" << std::endl;
	}
};

class TestClient : public SunNet::ChanneledClient<SunNet::TCPSocketConnection> {
public:
	TestClient(int poll_timeout) : ChanneledClient<SunNet::TCPSocketConnection>(poll_timeout) {}

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

void server_callback(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<GeneralMessage> ptr) {
	std::cout << "Server received: " << ptr->msg << std::endl;

	GeneralMessage response;
	response.msg = 12345678;
	sender->channeled_send<GeneralMessage>(&response);
}

void client_callback(SunNet::ChanneledSocketConnection_p sender, std::shared_ptr<GeneralMessage> ptr) {
	std::cout << "Client received: " << ptr->msg << std::endl;

	GeneralMessage response{ 1337 };
	sender->channeled_send(&response);
}

int main(int argc, char* argv[]) {

	SunNet::Channels::addNewChannel<GeneralMessage>();

	TestServer server("0.0.0.0", "9876", 5);
	server.open();
	server.serve(1000 * 2);

	server.subscribe<GeneralMessage>(server_callback);

	TestClient client(1000 * 2);
	client.connect("127.0.0.1", "9876");
	client.subscribe<GeneralMessage>(client_callback);

	struct GeneralMessage msg;
	msg.msg = 1337;

	client.channeled_send<GeneralMessage>(&msg);

	std::string dummy;
	std::getline(std::cin, dummy);
}
