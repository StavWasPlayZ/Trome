#pragma once

#include "handler/IRequestHandler.h"
#include <future>
#include <functional>

/**
 * A client in the Trivia server.
 * Holds technical information about the connection between this server and the client.
 * 
 * T - The platform socket type
 */
template <typename T>
class Client
{
public:
	Client(const T socket, IRequestHandler* const requestHandler, const std::function<void()> clientThreadFunc) :
		socket(socket),
		requestHandler(requestHandler)
	{
		thread = std::async(std::launch::async, clientThreadFunc);
	}

	~Client()
	{
		delete this->requestHandler;
	}

	const T socket;
	IRequestHandler* requestHandler;

	std::future<void>& getThread()
	{
		return this->thread;
	}

private:
	std::future<void> thread;
};
