#pragma once

#include "handler/IRequestHandler.h"
#include <future>
#include <functional>
#include <mutex>

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
    std::unique_lock<std::mutex> acquireSocketWriterLock();

    const IRequestHandler* getRequestHandler() const;
    void setRequestHandler(const IRequestHandler* requestHandler);
    std::unique_lock<std::mutex> acquireRequestHandlerLock();

    const std::future<void>& getThread() const;
    void setAndStartThread(const std::function<void()>& threadFunc);


    void sendNotification(const ProtocolNotification& notification);


    void handleDisconnecting() const;

private:
	const std::future<void>* thread;

    std::mutex requestHandlerMutex;
	const IRequestHandler* requestHandler;

    std::mutex socketWriterMutex;
};
