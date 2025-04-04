#pragma once

#include <iostream>
#include <stdexcept>

#include <chrono>

#include "Constants.h"

#include "exception/ForcedDisconnectionException.h"
#include "exception/SocketTimeoutException.h"

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

#include "handler/codec/s2c/Response.h"

#include "handler/codec/c2s/JsonRequestPacketDeserializer.h"
#include "handler/codec/s2c/JsonResponsePacketSerializer.h"
#include "request/RequestInfo.h"

#include "handler/LoginRequestHandler.h"

// For commonSetup and alike
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
	virtual void bindAndListen()
	{
		commonSetup();
		startServerThreads();
	}

	void close()
	{
		if (!this->_running)
			return;
	
		// Notify all threads that the server is closing
		this->_running = false;

		// Release the client cleaner thread
		this->_disconectedClientConditionalVariable.notify_all();
	
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
	/**
	* Contains all active clients.
	* 
	* Maps their socket address to their Client instantiation.
	*/
	std::map<T, Client<T>*> m_clients;

	/**
	 * The binding & listening process code common to all OSs
	 */
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

	/**
	 * Registers the provided socket as a client to the internal m_clients map.
	 * 
	 * The client is initiated with the LoginRequestHandler state.
	 */
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
	void startServerThreads()
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
	}


	/**
	* Returns true whether the message did not time out.
	*/
	virtual void recieveMsg(const T socket, void* buffer, const int length) const = 0;

	void sendMsg(const T socket, const unsigned char* buffer, const int length) const
	{
		bool didError;

		try
		{
			didError = send(socket, buffer, length, 0) == -1;
		}
		catch (...)
		{
			didError = true;
		}

		if (didError)
		{
			throwPlatformError("Failed to send message to client socket " + std::to_string(socket));
			throw std::exception();
		}
	}
	

	/**
	 * Platform-specific method for closing the server communication.
	 */
	virtual void platformClose() = 0;
	virtual void closeClientSocket(const T socket) = 0;

	/**
	 * Throws an exception with respect to the platform's preferred error type.
	 */
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

	std::condition_variable _disconectedClientConditionalVariable;
	std::mutex _disconectedClient_mutex;

	//SECTION Thread Functions

	void _serverThreadFunc()
	{
		while (this->_running)
		{
			acceptClients();
		}
	
		close();
	}

	void _clientThreadFunc(const T socket)
	{
		while (this->_running)
		{
			try
			{
				_handleClient(socket);
			}
			catch (const SocketTimeoutException& e)
			{
				// If we timed out (see RECV_REFRESH_TIMEOUT),
				// simply wait for the next recv cycle (if applicable).
				continue;
			}
			catch (const ForcedDisconnectionException& e)
			{
				break;
			}
			catch (const std::exception& e)
			{
				std::cerr << "Unknown exception occured (" << e.what() << "); Assuming client disconnection" << std::endl;
				break;
			}
		}
	
		_enqueueDisconnectClient(socket);
	}

	//ANCHOR This is where we actually process the client sockets.
	void _handleClient(const T socket)
	{
		const RequestInfo info = _waitForClientRequest(socket);

		Client<T>* client = this->m_clients.at(socket);
		const IRequestHandler* const handler = client->requestHandler;

		OBuffer responseBuffer;

		if (!handler->isRequestRelevant(info))
		{
			responseBuffer = JsonResponsePacketSerializer::serializeResponse(
				ErrorResponse("Illegal request")
			);
		}
		else
		{
			const RequestResult result = handler->handleRequest(info);

			// The Handler did its job well.
			// 🫡
			delete handler;

			responseBuffer = result.response;
			client->requestHandler = result.newHandler;
		}

		sendMsg(socket, responseBuffer.contents, responseBuffer.length);
		responseBuffer.freeContents();
	}

	RequestInfo _waitForClientRequest(const T socket)
	{
		unsigned char reqCode;
		recieveMsg(socket, &reqCode, SIZE_CODE);

		int jsonLen;
		recieveMsg(socket, &jsonLen, SIZE_JSON_LEN);
		jsonLen *= sizeof(char);

		if (jsonLen <= 0)
		{
			throw std::runtime_error("Invalid JSON length");
		}

		unsigned char* const data = new unsigned char[jsonLen + 1]; // +1 for null termination (better be safe than sorry).
		recieveMsg(socket, data, jsonLen + 1);

		const RequestInfo info(
			(ProtocolCode)reqCode,
			std::chrono::system_clock::to_time_t(
				std::chrono::system_clock::now()
			),
			JsonRequestPacketDeserializer::readJson(data, jsonLen)
		);

		delete[] data;

		return info;
	}

	void _clientCleanerThreadFunc()
	{
		std::unique_lock<std::mutex> lock(this->_disconectedClient_mutex);
		
		while (this->_running)
		{
			// waits for _enqueueDisconnectClient to be called
			this->_disconectedClientConditionalVariable.wait(lock);

			_freeDisconnectedClients();
		}
	}

	//!SECTION

	void _enqueueDisconnectClient(const T socket)
	{	
		std::cout << "Socket " << std::to_string(socket) << " disconected" << std::endl;

		this->_disconnectingClients_mutex.lock();
		this->_disconnectingClients.push_back(socket);
		this->_disconnectingClients_mutex.unlock();
		
		this->_disconectedClientConditionalVariable.notify_one();
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
		
		this->_disconnectingClients.clear();
		this->_disconnectingClients_mutex.unlock();
	}
};
