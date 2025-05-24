#include "Client.h"

#include "Server.h"
#include "handler/codec/s2c/notification/NotificationPacketSerializer.h"

Client::Client(const SOCKET socket, const IRequestHandler *const requestHandler) :
    socket(socket),
    thread(nullptr),
    requestHandler(requestHandler)
{}

Client::~Client()
{
    delete this->requestHandler;
    delete this->thread;
}

std::unique_lock<std::mutex> Client::acquireSocketWriterLock()
{
    return std::unique_lock(this->socketWriterMutex);
}

const IRequestHandler *Client::getRequestHandler() const
{
    return this->requestHandler;
}

void Client::setRequestHandler(const IRequestHandler *const requestHandler)
{
    this->requestHandler = requestHandler;
}

std::unique_lock<std::mutex> Client::acquireRequestHandlerLock()
{
    return std::unique_lock(this->requestHandlerMutex);
}

const std::future<void> &Client::getThread() const
{
    return *this->thread;
}

void Client::setAndStartThread(const std::function<void()> &threadFunc)
{
    if (this->thread != nullptr)
    {
        throw std::runtime_error("Client thread already set");
    }

    this->thread = new std::future(std::async(std::launch::async, threadFunc));
}

void Client::sendNotification(const ProtocolNotification& notification)
{
    Communicator::getInstance().sendMsg(*this,
        NotificationPacketSerializer::serialize(notification)
    );
}

void Client::handleDisconnecting() const
{
    Server::getInstance().getLoginManager().logout(*this);
}
