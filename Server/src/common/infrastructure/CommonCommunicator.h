#pragma once

#include <string>
#include <map>
#include <list>

#include <mutex>
// Much (much) better than threads in modern C++, and this use case in particular.
#include <future>
// It was suggested online to use this when sharing a resource.
#include <atomic>

#include "infrastructure/Client.h"

#include "handler/RequestHandlerFactory.h"
#include "request/RequestInfo.h"


// For commonSetup and alike.
// Also for sockaddr_in
#ifdef _WIN32
	#include <winsock2.h>
#else
#include <netinet/in.h>
#endif


class CommonCommunicator
{
public:
	CommonCommunicator(SOCKET defaultSocket, const RequestHandlerFactory& handlerFactory);
	virtual ~CommonCommunicator();

	bool isRunning() const;

	/**
	 * Binds this instance to the program port, and begins to listen for new clients.
	 * Returns: The future handling the client sockets.
	 * Completes when server closes.
	 */
	virtual void bindAndListen();

	void close();

protected:
	static constexpr unsigned int PORT = 6942;

	/**
	 * The timeout for the recv method.
	 * Set in place to allow refreshing the value of _running.
	 */
	static constexpr unsigned int RECV_REFRESH_TIMEOUT_MS = 3000;

	std::atomic<bool> _running;
	std::future<void> _serverThread;

	sockaddr_in _serverSockAddr;

	std::mutex m_clients_mutex;
	// Holding Client pointers because futures are immovable.
	/**
	 * Contains all active clients.
	 * Maps their socket address to their Client instantiation.
	 */
	std::map<SOCKET, Client*> m_clients;

	/**
	 * The binding & listening process code common to all OSs
	 */
	void commonSetup();

	virtual bool isValidSocket(SOCKET result) const = 0;
	virtual bool isValidBind(SOCKET result) const = 0;
	virtual bool isValidListen(SOCKET result) const = 0;
	virtual void setRecvTimeout(unsigned int timeoutMs) const = 0;
	virtual void acceptClients() = 0;

	/**
	 * Registers the provided socket as a client to the internal m_clients map.
	 * The client is initiated with the LoginRequestHandler state.
	 */
	void registerClient(SOCKET socket);

	/**
	 * Returns: The future handling the client sockets.
	 * Completes when server closes.
	 */
	void startServerThreads();

	/**
	 * Returns true whether the message did not time out.
	 */
	virtual void receiveMsg(SOCKET socket, void* buffer, int length) const = 0;

	void sendMsg(SOCKET socket, const unsigned char* buffer, int length) const;

	/**
	 * Platform-specific method for closing the server communication.
	 */
	virtual void platformClose() = 0;
	virtual void closeClientSocket(SOCKET socket) = 0;

	/**
	 * Throws an exception with respect to the platform's preferred error type.
	 */
	virtual void throwPlatformError(const std::string& msg) const;

	SOCKET m_serverSocket;

private:
	const RequestHandlerFactory m_handlerFactory;

	std::mutex _disconnectingClients_mutex;
	
	/**
	 * A list containing all clients that need to be disconnected
	 */
	std::list<SOCKET> _disconnectingClients;

	std::condition_variable _disconnectedClientConditionalVariable;
	std::mutex _disconnectedClient_mutex;

	//SECTION Thread Functions

	void _serverThreadFunc();
	void _clientThreadFunc(SOCKET socket);

	//ANCHOR Actual client processing function.
	void _handleClient(SOCKET socket) const;

	RequestInfo _waitForClientRequest(SOCKET socket) const;
	void _clientCleanerThreadFunc();

	//!SECTION

	void _enqueueDisconnectClient(SOCKET socket);
	void _freeDisconnectedClients();
};
