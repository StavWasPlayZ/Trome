#pragma once

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

#include <string>
#include <map>
#include <list>

#include <mutex>
// Much (much) better than threads in modern C++, and this usecase in particular.
#include <future>
// It was suggested online to use this when sharing a resource.
#include <atomic>

#include "Client.h"

class Communicator
{
public:
	Communicator();
	~Communicator();

	/**
	* Binds this instance to the program port, and begins to listen for new clients.
	*
	* Returns: The future handling the client sockets.
	* Completes when server closes.
	*/
	std::future<void>& bindAndListen();
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

	SOCKET m_serverSocket;
	struct sockaddr_in _address;

	std::atomic<bool> _running;
	std::future<void> _serverThread;

	std::mutex m_clients_mutex;
	// Holding Client pointers because futures are immovable.
	std::map<SOCKET, Client*> m_clients;

	std::mutex _disconnectingClients_mutex;
	/**
	* A list containing all clients that need to be disconnected
	*/
	std::list<SOCKET> _disconnectingClients;


	void _serverThreadFunc();
	void _freeDisconnectedClients();

	void _acceptClients();
	void _clientThreadFunc(const SOCKET socket);
	void _disconnectClient(const SOCKET socket);

	/**
	* Returns true whether the message did not time out.
	*/
	bool recieveMsg(const SOCKET socket, char* buffer, const int length, const int flags) const;
	void sendMsg(const SOCKET socket, const char* buffer, const int length, const int flags) const;
};