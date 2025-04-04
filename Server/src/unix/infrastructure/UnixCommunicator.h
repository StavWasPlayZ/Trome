#pragma once

#include "infrastructure/CommonCommunicator.h"

class UnixCommunicator : public CommonCommunicator<int>
{
public:
	UnixCommunicator();

protected:
	virtual bool isValidSocket(const int result) const override;
	virtual bool isValidBind(const int result) const override;
	virtual bool isValidListen(const int result) const override;

	virtual void setRecvTimeout(const unsigned int timeoutMs) const override;
	virtual void recieveMsg(const int socket, void* buffer, const int length) const override;

	virtual void platformClose() override;
	virtual void closeClientSocket(const int socket) override;

	virtual void acceptClients() override;

	virtual void throwPlatformError(const std::string& msg) const override;
};