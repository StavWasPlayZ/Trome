#pragma once

#include "infrastructure/Communicator.hpp"

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

class WindowsCommunicator : public Communicator<SOCKET>
{
public:
	WindowsCommunicator();

	/**
	* Binds this instance to the program port, and begins to listen for new clients.
	*
	* Returns: The future handling the client sockets.
	* Completes when server closes.
	*/
	virtual std::future<void>& bindAndListen() override;

protected:
	virtual void acceptClients() override;

	virtual bool recieveMsg(const SOCKET socket, char* buffer, const int length) const;
	virtual void sendMsg(const SOCKET socket, const char* buffer, const int length) const;

	virtual void platformClose() override;
	virtual void closeClientSocket(const SOCKET socket) override;

private:
	SOCKET m_serverSocket;
	struct sockaddr_in _address;
};