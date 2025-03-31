#pragma once

#include "infrastructure/Communicator.hpp"

class LinuxCommunicator : public Communicator<int>
{
public:
	LinuxCommunicator();

	/**
	* Binds this instance to the program port, and begins to listen for new clients.
	*
	* Returns: The future handling the client sockets.
	* Completes when server closes.
	*/
	virtual std::future<void>& bindAndListen() override;

protected:
	virtual void acceptClients() override;

	virtual bool recieveMsg(const int socket, char* buffer, const int length) const;
	virtual void sendMsg(const int socket, const char* buffer, const int length) const;

	virtual void platformClose() override;
	virtual void closeClientSocket(const int socket) override;

private:
	int m_serverSocket;
};