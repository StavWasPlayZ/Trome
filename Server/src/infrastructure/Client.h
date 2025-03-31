#pragma once

#include "handler/IRequestHandler.h"
#include <WinSock2.h>
#include <future>
#include <functional>

class Client
{
public:
	Client(const SOCKET socket, IRequestHandler* const requestHandler, const std::function<void()> clientThreadFunc);
	~Client();

	const SOCKET socket;
	IRequestHandler* requestHandler;

	std::future<void>& getThread();

private:
	std::future<void> thread;
};
