#pragma once

#include "handler/IRequestHandler.h"
#include <future>
#include <functional>

#ifndef _WIN32
// In Unix, socket = int.
#define SOCKET int
#else
// To have SOCKET, we need WinSock.
#include <WinSock2.h>
#endif

class IRequestHandler;

/**
 * A client in the Trivia server.
 * Holds technical information about the connection between this server and the client.
 */
class Client
{
public:
	Client(SOCKET socket, const IRequestHandler* requestHandler);
	~Client();

	const SOCKET socket;
	const IRequestHandler* requestHandler;

    const std::future<void>& getThread() const;
    void setAndStartThread(const std::function<void()>& threadFunc);

private:
	const std::future<void>* thread;
};
