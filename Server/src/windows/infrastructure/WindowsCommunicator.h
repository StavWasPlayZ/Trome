#pragma once

#include "infrastructure/CommonCommunicator.h"

#pragma comment(lib, "ws2_32.lib")

#include <winsock2.h>
#include <ws2tcpip.h>

class WindowsCommunicator : public CommonCommunicator
{
public:
	WindowsCommunicator(const RequestHandlerFactory& handlerFactory);

	/**
	* Binds this instance to the program port, and begins to listen for new clients.
	*
	* Returns: The future handling the client sockets.
	* Completes when server closes.
	*/
	virtual void bindAndListen() override;

protected:
	virtual bool isValidSocket(const SOCKET result) const override;
	virtual bool isValidBind(const SOCKET result) const override;
	virtual bool isValidListen(const SOCKET result) const override;

	virtual void acceptClients() override;

	virtual void setRecvTimeout(const unsigned int timeoutMs) const override;
	virtual void recieveMsg(const SOCKET socket, void* buffer, const int length) const override;

	virtual void platformClose() override;
	virtual void closeClientSocket(const SOCKET socket) override;

	void throwPlatformError(const std::string &msg) const override;
};