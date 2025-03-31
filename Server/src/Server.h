#pragma once

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>

// Much (much) better than threads in modern C++, and this usecase in particular.
#include <future>
// It was suggested online to use this when sharing a resource.
#include <atomic>

class Server
{
public:
	Server();
	~Server();

	/**
	* Runs the server.
	* 
	* Returns: The future handling the client sockets.
	* Completes when server closes.
	*/
	std::future<void>& run();
	void close();

	bool isRunning() const;

private:
	static constexpr unsigned int PORT = 6942;
	static const std::string CMD_HELLO;

	/**
	* The timeout for the recv method.
	* Set in place to allow refreshing the value of _running.
	*/
	static constexpr unsigned int RECV_REFRESH_TIMEOUT = 3000;

	SOCKET _serverSocket;
	struct sockaddr_in _address;

	std::atomic<bool> _running;
	std::future<void> _serverThread;
	std::vector<std::future<void>> _clientThreads;


	void _acceptClients();
	void _handleClient(const SOCKET socket) const;
};