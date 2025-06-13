#include "Client.h"

#include "Server.h"
#include "handler/codec/s2c/notification/NotificationPacketSerializer.h"

Client::Client(const SOCKET socket, const IRequestHandler *const requestHandler) :
    socket(socket),
    requestHandler(requestHandler)
{}

Client::~Client()
{
    delete this->requestHandler;
}

std::unique_lock<std::mutex> Client::acquireSocketWriterLock()
{
    return std::unique_lock(this->socketWriterMutex);
}

const IRequestHandler *Client::getRequestHandler() const
{
    return this->requestHandler;
}

void Client::setRequestHandlerUnsafe(const IRequestHandler *const requestHandler)
{
    this->requestHandler = requestHandler;
}

void Client::setRequestHandlerSafe(const IRequestHandler *requestHandler)
{
    std::unique_lock<std::mutex> handlerLock = acquireRequestHandlerLock();

    delete getRequestHandler();
    setRequestHandlerUnsafe(requestHandler);
}

std::unique_lock<std::mutex> Client::acquireRequestHandlerLock()
{
    return std::unique_lock(this->requestHandlerMutex);
}

void Client::setAndStartThread(const std::function<void()> &threadFunc)
{
    if (thread.valid())
    {
        throw std::runtime_error("already started");
    }

    this->thread = std::async(std::launch::async, threadFunc);
}

void Client::waitForExit()
{
    if (!thread.valid())
        return;

    thread.get();
}

void Client::sendNotification(const ProtocolNotification& notification)
{
    Communicator::getInstance().sendMsg(*this,
        NotificationPacketSerializer::serialize(notification)
    );
}

void Client::handleDisconnecting() const
{
    Server::getInstance().getLoginManager().getUserByClient(*this).handleDisconnecting();
}
