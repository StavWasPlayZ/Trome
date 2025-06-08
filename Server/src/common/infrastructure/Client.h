#pragma once

#include "handler/IRequestHandler.h"
#include "cryptoAlgorithm/ICryptoAlgorithm.h"
#include "cryptoAlgorithm/NoEncryption.h"
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

struct ProtocolNotification;
class IRequestHandler;
class ICryptoAlgorithm;

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
    /**
     * Does not lock other threads from using the handler.
     */
    void setRequestHandlerUnsafe(const IRequestHandler* requestHandler);
    /**
     * Locks other threads from using the handler.
     */
    void setRequestHandlerSafe(const IRequestHandler* requestHandler);

    std::unique_lock<std::mutex> acquireRequestHandlerLock();


    const std::future<void>& getThread() const;
    void setAndStartThread(const std::function<void()>& threadFunc);


    void sendNotification(const ProtocolNotification& notification);


    void handleDisconnecting() const;

    ICryptoAlgorithm* getCryptoAlgorithm() const;

private:
	const std::future<void>* thread;

    std::mutex requestHandlerMutex;
	const IRequestHandler* requestHandler;

    std::mutex socketWriterMutex;

    ICryptoAlgorithm* cryptoAlgorithem;
};
