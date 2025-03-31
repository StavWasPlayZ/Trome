#pragma once

#include "infrastructure/CommonCommunicator.hpp"

class LinuxCommunicator : public CommonCommunicator<int>
{
public:
	LinuxCommunicator();

protected:
	virtual bool isValidSocket(const int result) const override;
	virtual bool isValidBind(const int result) const override;
	virtual bool isValidListen(const int result) const override;

	virtual void setRecvTimeout(const unsigned int timeoutMs) const override;
	virtual bool recieveMsg(const int socket, char* buffer, const int length) const override;

	virtual void platformClose() override;
	virtual void closeClientSocket(const int socket) override;

	virtual void acceptClients() override;

	virtual void throwPlatformError(const std::string& msg) const override;
};