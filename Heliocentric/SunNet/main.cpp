#include "tcp_socket_connection.hpp"
#include "pollservice.h"

#include <memory>
#include <iostream>
#include <string>

using namespace Sunnet;
using TCPSocketConnection_p = std::shared_ptr<TCPSocketConnection>;

void operate(SocketCollection_p sockets) {
	for (Socket_iter it = sockets->begin(); it != sockets->end(); ++it) {
		SocketConnection_p socket = *it;
		char data[4];
		socket->receive(data, 3);
		data[3] = '\0';
		std::cout << "RECEIVED: " << data << std::endl;
	}
}

int main(int argc, char* argv[]) {
	TCPSocketConnection_p server = std::make_shared<TCPSocketConnection>();
	TCPSocketConnection_p client = std::make_shared<TCPSocketConnection>();

	server->bind("9876");
	server->listen(5);

	std::cout << "hosting on localhost, port " << 9876 << std::endl;

	client->connect("127.0.0.1", "9876");

	SocketConnection_p server_client = server->accept();
	std::string message = "Hello, world! (From server)";
	server_client->send(message.c_str(), message.size());

	char data[512];
	std::memset(data, 0, 512);

	client->receive(data, message.size());
	std::cout << data << std::endl;

	std::memset(data, 0, 512);
	message = "Hello, world! (From client)";
	client->send(message.c_str(), message.size());

	server_client->receive(data, message.size());
	std::cout << data << std::endl;

	std::vector<SocketConnection_p> vec;
	vec.push_back(server_client);

	SocketCollection_p clients = std::make_shared<SocketCollection>(vec.begin(), vec.end());
	PollService poll_service(clients, 1000 * 2);

	/* Should timeout */
	std::cout << "Polling.. should probably block" << std::endl;
	SocketCollection_p ready_socks = poll_service.poll();
	operate(ready_socks);

	std::cout << "Polling.. should not block" << std::endl;
	client->send("abc", 3);

	/* Should not timeout */
	ready_socks = poll_service.poll();
	operate(ready_socks);

	std::string dummy;
	std::cin >> dummy;
}