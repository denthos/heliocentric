#include "tcp_socket_connection.hpp"

#include <memory>
#include <iostream>
#include <string>

using namespace Sunnet;
using TCPSocketConnection_p = std::shared_ptr<TCPSocketConnection>;
using SocketConnection_p = std::shared_ptr<SocketConnection>;

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

	std::string dummy;
	std::cin >> dummy;
}