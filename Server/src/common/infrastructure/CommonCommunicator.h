#pragma once

#include <stdexcept>

#include <string>
#include <map>
#include <list>

#include <mutex>
// Much (much) better than threads in modern C++, and this usecase in particular.
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
	#include <sys/socket.h>
	#include <netinet/in.h>
#endif


/**
 * T - The platform socket address type
 */
template <typename T>
class CommonCommunicator
{
public:
	CommonCommunicator(const T defaultSocket, const RequestHandlerFactory& handlerFactory);
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
	static inline const std::string CMD_HELLO = "Hello";

	/**
	 * The timeout for the recv method.
	 * Set in place to allow refreshing the value of _running.
	 */
	static constexpr unsigned int RECV_REFRESH_TIMEOUT_MS = 3000;

	std::atomic<bool> _running;
	std::future<void> _serverThread;

	struct sockaddr_in _serverSockAddr;

	std::mutex m_clients_mutex;
	// Holding Client pointers because futures are immovable.
	/**
	 * Contains all active clients.
	 * Maps their socket address to their Client instantiation.
	 */
	std::map<T, Client<T>*> m_clients;

	/**
	 * The binding & listening process code common to all OSs
	 */
	void commonSetup();

	virtual bool isValidSocket(const T result) const = 0;
	virtual bool isValidBind(const T result) const = 0;
	virtual bool isValidListen(const T result) const = 0;
	virtual void setRecvTimeout(const unsigned int timeoutMs) const = 0;
	virtual void acceptClients() = 0;

	/**
	 * Registers the provided socket as a client to the internal m_clients map.
	 * The client is initiated with the LoginRequestHandler state.
	 */
	void registerClient(const T socket);

	/**
	 * Returns: The future handling the client sockets.
	 * Completes when server closes.
	 */
	void startServerThreads();

	/**
	 * Returns true whether the message did not time out.
	 */
	virtual void recieveMsg(const T socket, void* buffer, const int length) const = 0;

	void sendMsg(const T socket, const unsigned char* buffer, const int length) const;

	/**
	 * Platform-specific method for closing the server communication.
	 */
	virtual void platformClose() = 0;
	virtual void closeClientSocket(const T socket) = 0;

	/**
	 * Throws an exception with respect to the platform's preferred error type.
	 */
	virtual void throwPlatformError(const std::string& msg) const;

	T m_serverSocket;

private:
	const RequestHandlerFactory m_handlerFactory;

	std::mutex _disconnectingClients_mutex;
	
	/**
	 * A list containing all clients that need to be disconnected
	 */
	std::list<T> _disconnectingClients;

	std::condition_variable _disconectedClientConditionalVariable;
	std::mutex _disconectedClient_mutex;

	//SECTION Thread Functions

	void _serverThreadFunc();
	void _clientThreadFunc(const T socket);

	//ANCHOR Actual client processing function.
	void _handleClient(const T socket);

	RequestInfo _waitForClientRequest(const T socket);
	void _clientCleanerThreadFunc();

	//!SECTION

	void _enqueueDisconnectClient(const T socket);
	void _freeDisconnectedClients();
};


#include "CommonCommunicator.tpp"
