#pragma once

#include <iostream>
#include <stdexcept>

#include <string>
#include <map>
#include <list>

#include <mutex>
// Much (much) better than threads in modern C++, and this usecase in particular.
#include <future>
// But this is also necessary for client cleaning thread.
#include <thread>
// It was suggested online to use this when sharing a resource.
#include <atomic>

#include "infrastructure/Client.hpp"

#include "handler/LoginRequestHandler.h"

// For commonSetup and alike
#ifdef _WIN32
	#include <winsock2.h>
#else
	#include <sys/socket.h>
	#include <netinet/in.h>
#endif


/**
 * T - The platform socket type
 */
template <typename T>
class CommonCommunicator
{
public:
	CommonCommunicator(const T defaultSocket) :
		m_serverSocket(defaultSocket),
		_serverSockAddr({ 0 }),
		_running(false)
	{}

	virtual ~CommonCommunicator()
	{
		close();
	}


	bool isRunning() const
	{
		return this->_running;
	}

	/**
	* Binds this instance to the program port, and begins to listen for new clients.
	*
	* Returns: The future handling the client sockets.
	* Completes when server closes.
	*/
	virtual std::future<void>& bindAndListen()
	{
		commonSetup();
		return startServerThreads();
	}

	void close()
	{
		if (!this->_running)
			return;
	
		// Notify all threads that the server is closing
		this->_running = false;
	
		// Wait for 'em to close
		this->m_clients_mutex.lock();
		for (const auto& client : this->m_clients)
		{
			client.second->thread.wait();
		}
		this->m_clients_mutex.unlock();

		
		platformClose();

		this->_serverSockAddr = { 0 };
		this->_serverThread = std::future<void>();
	}

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
	std::map<T, Client<T>*> m_clients;


	void commonSetup()
	{
		this->m_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (!isValidSocket(this->m_serverSocket))
		{
			close();
			throwPlatformError("Socket creation failed");
			throw std::exception();
		}
	
		// The timeout for the recv method
		// Set in place to allow refreshing the value of _running.
		setRecvTimeout(RECV_REFRESH_TIMEOUT_MS);
	
		// Set server address information
		this->_serverSockAddr.sin_family = AF_INET;
		this->_serverSockAddr.sin_addr.s_addr = INADDR_ANY;
		this->_serverSockAddr.sin_port = htons(PORT);
	
		if (!isValidBind(
			bind(this->m_serverSocket, (struct sockaddr*)&this->_serverSockAddr, sizeof(this->_serverSockAddr))
		)) {
			close();
			throwPlatformError("Binding failed");
			throw std::exception();
		}
	
		this->_running = true;
	
		// Start listening for connections
		if (!isValidListen(listen(this->m_serverSocket, 3)))
		{
			close();
			throwPlatformError("Listen failed");
			throw std::exception();
		}
	}

	virtual bool isValidSocket(const T result) const = 0;
	virtual bool isValidBind(const T result) const = 0;
	virtual bool isValidListen(const T result) const = 0;

	virtual void setRecvTimeout(const unsigned int timeoutMs) const = 0;


	virtual void acceptClients() = 0;

	void registerClient(const T socket)
	{
		this->m_clients[socket] = new Client<T>(
			socket,
			new LoginRequestHandler(),
			[this, socket]()
			{
				_clientThreadFunc(socket);
			}
		);

		std::cout << "Connection accepted from " + std::to_string(socket) << std::endl;
	}


	/**
	* Returns: The future handling the client sockets.
	* Completes when server closes.
	*/
	std::future<void>& startServerThreads()
	{
		this->_serverThread = std::async(
			std::launch::async,
			[this]()
			{
				_serverThreadFunc();
			}
		);

		// Also start client cleaner thread
		std::thread(
			[this]()
			{
				_clientCleanerThreadFunc();
			}
		).detach();

		std::cout << "Listening on port " << PORT << "..." << std::endl;

		return this->_serverThread;
	}


	/**
	* Returns true whether the message did not time out.
	*/
	virtual bool recieveMsg(const T socket, char* buffer, const int length) const = 0;
	void sendMsg(const T socket, const char* buffer, const int length) const
	{
		if (send(socket, buffer, length, 0) == -1)
		{
			throwPlatformError("Failed to send message to client socket " + std::to_string(socket));
		}
	}
	

	virtual void platformClose() = 0;
	virtual void closeClientSocket(const T socket) = 0;

	virtual void throwPlatformError(const std::string& msg) const
	{
		throw std::runtime_error(msg);
	}

	T m_serverSocket;

private:
	std::mutex _disconnectingClients_mutex;
	/**
	* A list containing all clients that need to be disconnected
	*/
	std::list<T> _disconnectingClients;

	//SECTION Thread Functions

	void _serverThreadFunc()
	{
		while (this->_running)
		{
			acceptClients();
		}
	
		close();
	}

	//ANCHOR This is where we actually process the client sockets.
	void _clientThreadFunc(const T socket)
	{
		sendMsg(socket, CMD_HELLO.c_str(), CMD_HELLO.length());
	
		while (this->_running)
		{
			char buffer[6];
			if (!recieveMsg(socket, buffer, sizeof(buffer)))
			{
				// If we timed out (see RECV_REFRESH_TIMEOUT),
				// simply wait for the next recv cycle (if applicable).
				continue;
			}
	
			buffer[5] = 0;
	
			if (buffer == CMD_HELLO)
			{
				sendMsg(socket, CMD_HELLO.c_str(), CMD_HELLO.length());
			}
		}
	
		_enqueueDisconnectClient(socket);
	}

	void _clientCleanerThreadFunc()
	{
		while (this->_running)
		{
			//TODO: Add that mutex where you tell it to be unlocked in some other thread and then
			// this thread is like "woah i need to wake up" kind of mutex
			_freeDisconnectedClients();
		}
	}

	//!SECTION

	void _enqueueDisconnectClient(const T socket)
	{	
		this->_disconnectingClients_mutex.lock();
		this->_disconnectingClients.push_back(socket);
		this->_disconnectingClients_mutex.unlock();
	}

	void _freeDisconnectedClients()
	{
		this->_disconnectingClients_mutex.lock();
		this->m_clients_mutex.lock();
	
		for (const auto& clientSock : this->_disconnectingClients)
		{
			delete this->m_clients.at(clientSock);
			m_clients.erase(clientSock);
		}
	
		this->m_clients_mutex.unlock();
		this->_disconnectingClients_mutex.unlock();
	}
};
