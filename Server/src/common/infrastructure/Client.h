#pragma once

#include "handler/IRequestHandler.h"
#include <future>
#include <functional>

/**
 * A client in the Trivia server.
 * Holds technical information about the connection between this server and the client.
 * 
 * T - The platform socket address type
 */
template <typename T>
class Client
{
public:
	Client(T socket, const IRequestHandler* requestHandler, const std::function<void()> &clientThreadFunc);
	~Client();

	const T socket;
	const IRequestHandler* requestHandler;

	/**
	* The Client thread of this user
	*/
	const std::future<void> thread;
};


#include "Client.tpp"
