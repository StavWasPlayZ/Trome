#pragma once

#include "handler/IRequestHandler.h"
#include <future>
#include <functional>

#ifndef _WIN32
// In Unix, socket = int.
#define SOCKET int
#endif

class IRequestHandler;

/**
 * A client in the Trivia server.
 * Holds technical information about the connection between this server and the client.
 */
class Client
{
public:
	Client(SOCKET socket, const IRequestHandler* requestHandler, const std::function<void()> &clientThreadFunc);
	~Client();

	const SOCKET socket;
	const IRequestHandler* requestHandler;

	/**
	* The Client thread of this user
	*/
	const std::future<void> thread;
};
