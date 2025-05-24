#pragma once

#include "infrastructure/CommonCommunicator.h"

class UnixCommunicator : public CommonCommunicator
{
public:
    UnixCommunicator(const UnixCommunicator&) = delete;
    void operator=(const UnixCommunicator&) = delete;

    static UnixCommunicator& getAndInitiateInstance(const RequestHandlerFactory *handlerFactory);
    static UnixCommunicator& getInstance();

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

private:
    explicit UnixCommunicator(const RequestHandlerFactory *handlerFactory);
};