#pragma once

#include "infrastructure/CommonCommunicator.h"

class UnixCommunicator : public CommonCommunicator
{
public:
	explicit UnixCommunicator(const RequestHandlerFactory& handlerFactory);

protected:
	bool isValidSocket(int result) const override;
	bool isValidBind(int result) const override;
	bool isValidListen(int result) const override;

	void setRecvTimeout(unsigned int timeoutMs) const override;
	void receiveMsg(int socket, void* buffer, int length) const override;

	void platformClose() override;
	void closeClientSocket(int socket) override;

	void acceptClients() override;

	void throwPlatformError(const std::string& msg) const override;
};