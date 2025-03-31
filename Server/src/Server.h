#pragma once

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>

class Server
{
public:
	Server();
	~Server();

	void run();
	void close();

private:
	static constexpr unsigned int PORT = 6942;
	static const std::string CMD_EXIT;
	static const std::string CMD_HELLO;

	SOCKET _serverSocket;
	struct sockaddr_in _address;

	void _acceptClients() const;
};